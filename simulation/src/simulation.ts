import { request } from 'graphql-request';
import { List, Map } from 'immutable';
import { isNumber } from 'util';
import { VM } from 'vm2';

const maxCyclesCount = 100;

export const executeSimulation = async (matchId: string) => {
  const match = await fetchMatch(matchId);

  let frameOrder = 0;

  const gameVM = new GameVM(match.game, List(match.scripts));

  while (!gameVM.isGameEnded() && gameVM.getExecutedCyclesCount() < maxCyclesCount) {
    const cycleResult = gameVM.executeCycle();

    const newFrames = cycleResult.frames.map((frame: any) => ({ order: frameOrder++, content: frame }));

    await provideResultFrames(matchId, newFrames);
  }

  const winner = gameVM.getWinner();
  if (isNumber(winner)) {
    await provideMatchWinner(matchId, winner);
  }
};

interface Match {
  game: Game;
  scripts: Script[];
}

interface Game {
  code: string;
}

interface Script {
  code: string;
}

interface ResultFrame {
  order: number;
  content: string;
}

interface GameSandbox {
  loadState: () => any;
  saveState: (state: any) => any;
  emitFrame: (frame: any) => any;
  end: (newWinderIndex: any) => void;
  runPlayerScript: (playerIndx: any, gameState: any) => any;
  getCycle: () => number;
}

interface PlayerSandbox {
  answer: (playerAnswer: any) => any;
  loadState: () => any;
  saveState: (state: any) => any;
  getGameState: () => any;
}

class GameVM {
  public readonly maxCycles = 100;
  public readonly gameCycleTimeout = 10000;
  public readonly playerAnswerTimeout = 1000;
  public readonly gameSandbox: GameSandbox;

  private gameState: any = null;
  private playerStates: Map<number, any> = Map();
  private frames: List<any> = List();
  private winner: number | null | undefined = undefined;
  private cycle: number = 0;

  constructor(private game: Game, private scripts: List<Script>) {
    this.gameSandbox = {
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
  }

  public executeCycle(): { frames: List<any> } {
    if (this.isGameEnded()) {
      throw new Error('Cannot execute cycle on ended game');
    }

    const vm = new VM({
      timeout: this.gameCycleTimeout,
      sandbox: this.gameSandbox
    });
    this.runVM(vm, this.game.code);

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

    const playerSandbox : PlayerSandbox = {
      answer: (playerAnswer: any) => (answer = playerAnswer),
      loadState: () => this.playerStates.get(playerIndex),
      saveState: (state: any) => (this.playerStates = this.playerStates.set(playerIndex, state)),
      getGameState: () => gameState,
    };

    const vm = new VM({
      timeout: this.playerAnswerTimeout,
      sandbox: playerSandbox
    });

    const script = this.scripts.get(playerIndex) as Script;
    this.runVM(vm, script.code);

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

const fetchMatch = async (matchId: string): Promise<Match> => {
  const query = `
    query($matchId: ID!) {
      match(matchId: $matchId) {
        game {
          code
        }
        scripts {
          code
        }
      }
    }`;

  const data = await request(process.env.GRAPHQL_URL as string, query, { matchId });

  return data.match as Match;
};

const provideResultFrames = async (matchId: string, resultFrames: List<ResultFrame>): Promise<boolean> => {
  const mutation = `
    mutation($matchId: ID!, $resultFrames: [NewResultFrame!]!) {
      provideResultFrames(matchId: $matchId, resultFrames: $resultFrames)
    }`;

  const data = await request(process.env.GRAPHQL_URL as string, mutation, { matchId, resultFrames });

  return data.provideResultFrames as boolean;
}

const provideMatchWinner = async (matchId: string, winner: number): Promise<boolean> => {
  const mutation = `
    mutation($matchId: ID!, $winner: Int!) {
      provideMatchWinner(matchId: $matchId, winner: $winner)
    }`;

  const data = await request(process.env.GRAPHQL_URL as string, mutation, { matchId, winner });

  return data.provideMatchWinner as boolean;
} 
