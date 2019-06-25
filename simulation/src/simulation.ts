import { ObjectID } from 'bson';
import { List, Map } from 'immutable';
import { isNumber } from 'util';
import { VM } from 'vm2';
import * as dao from './dao';
import { Game, IGame } from './model/game';
import { Match } from './model/match';
import { IScript, Script } from './model/script';

const maxCyclesCount = 100;

export const executeSimulation = async (matchId: ObjectID) => {
  const match = await dao.findById(matchId, Match);
  const game = await dao.findById(match.game, Game);
  const scripts = await dao.findByIds(match.scripts, Script);

  let frameOrder = 0;

  const gameVM = new GameVM(game, scripts);

  match.result = {
    winner: null,
    resultFrames: [],
  };

  while (!gameVM.isGameEnded() && gameVM.getExecutedCyclesCount() < maxCyclesCount) {
    const cycleResult = gameVM.executeCycle();

    const newFrames = cycleResult.frames.map((frame: any) => ({ order: frameOrder++, content: frame }));
    
    try {
      match.result.resultFrames.push(...newFrames);
      await dao.save(match);
    } catch (err) {
      console.error('Cannot save result frames. Probably frame has invalid format. Original error: ' + err);
      return;
    }
  }

  match.result.winner = gameVM.getWinner();
  await dao.save(match);
};

class GameVM {
  public readonly maxCycles = 100;
  public readonly gameCycleTimeout = 10000;
  public readonly playerAnswerTimeout = 1000;
  public readonly vm: VM;

  private gameState: any = null;
  private playerStates: Map<number, any> = Map();
  private frames: List<any> = List();
  private winner: number | null | undefined = undefined;
  private cycle: number = 0;

  constructor(private game: IGame, private scripts: List<IScript>) {
    const sandbox = {
      loadState: () => this.gameState,
      saveState: (state: any) => (this.gameState = state),
      emitFrame: (frame: any) => (this.frames = this.frames.push(frame)),
      end: (newWinner: any) => {
        this.validatePlayerIndex(newWinner);
        this.winner = newWinner;
      },
      runPlayerScript: (playerIndex: any, gameState: any) => {
        this.validatePlayerIndex(playerIndex);
        return this.runPlayerScript(playerIndex, gameState);
      },
      getCycle: () => this.cycle,
    };

    this.vm = new VM({
      timeout: this.gameCycleTimeout,
      sandbox,
    });
  }

  public executeCycle(): { frames: List<any> } {
    if (this.isGameEnded()) {
      throw new Error('Cannot execute cycle on ended game');
    }

    this.runVM(this.vm, this.game.mainLoop);

    const result = {
      frames: this.frames,
    };

    this.cycle++;
    this.frames = List();

    return result;
  }

  public getExecutedCyclesCount(): number {
    return this.cycle;
  }

  public isGameEnded(): boolean {
    return this.winner !== undefined;
  }

  public getWinner(): number | null {
    return isNumber(this.winner) ? this.winner : null;
  }

  private validatePlayerIndex(playerIndex: any): void {
    if (!(Number.isInteger(playerIndex) && playerIndex >= 0 && playerIndex < this.scripts.size)) {
      throw new Error('Bad player index');
    }
  }

  private runPlayerScript(playerIndex: number, gameState: any): any {
    let answer: any;
    const vm = new VM({
      timeout: this.playerAnswerTimeout,
      sandbox: {
        answer: (playerAnswer: any) => (answer = playerAnswer),
        loadState: () => this.playerStates.get(playerIndex),
        saveState: (state: any) => (this.playerStates = this.playerStates.set(playerIndex, state)),
        getGameState: () => gameState,
      },
    });

    const script = this.scripts.get(playerIndex) as IScript;
    this.runVM(vm, script.content);

    return answer;
  }

  private runVM(vm: VM, script: string) {
    try {
      vm.run(script);
    } catch (err) {
      console.error('Error occured while running vm script: ' + err);
    }
  }
}
