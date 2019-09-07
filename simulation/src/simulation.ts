import { request } from 'graphql-request';
import { List, Map } from 'immutable';
import { isNumber } from 'util';
import { VM } from 'vm2';

export const executeSimulation = async (matchId: string) => {
  const match = await fetchMatch(matchId);

  let frameOrder = 0;

  const onEmitFrame = (frame: string) => provideResultFrames(matchId, List([{ order: frameOrder++, content: frame }]));

  const gameVM = new GameVM(match.game, List(match.scripts), onEmitFrame);
  gameVM.run();

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
  emitFrame: (frame: any) => any;
  end: (newWinderIndex: any) => void;
  runPlayerScript: (playerIndx: any, gameState: any) => any;
}

interface PlayerSandbox {
  answer: (playerAnswer: any) => any;
  loadState: () => any;
  saveState: (state: any) => any;
  getGameState: () => any;
}

class GameVM {
  public readonly gameTimeout = 10000;
  public readonly playerAnswerTimeout = 1000;
  public readonly gameSandbox: GameSandbox;

  private playerStates: Map<number, any> = Map();
  private winner: number | null | undefined = undefined;

  constructor(private game: Game, private scripts: List<Script>, onEmitFrame: (frame: string) => void) {
    this.gameSandbox = {
      emitFrame: (frame: any) => {
        const frameAsJson = this.convertToJson(frame);
        if (frameAsJson !== null) {
          onEmitFrame(frameAsJson);
        }
      },
      end: (newWinner: any) => {
        this.validatePlayerIndex(newWinner);
        this.winner = newWinner;
      },
      runPlayerScript: (playerIndex: any, gameState: any) => {
        this.validatePlayerIndex(playerIndex);
        return this.runPlayerScript(playerIndex, gameState);
      },
    };
  }

  public run(): void {
    if (this.isGameEnded()) {
      throw new Error('Cannot run game twice');
    }

    const vm = new VM({
      timeout: this.gameTimeout,
      sandbox: this.gameSandbox,
    });
    this.runVM(vm, this.game.code);

    if (this.winner === undefined) {
      this.winner = null;
    }
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

    const playerSandbox: PlayerSandbox = {
      answer: (playerAnswer: any) => (answer = playerAnswer),
      loadState: () => this.playerStates.get(playerIndex),
      saveState: (state: any) => (this.playerStates = this.playerStates.set(playerIndex, state)),
      getGameState: () => gameState,
    };

    const vm = new VM({
      timeout: this.playerAnswerTimeout,
      sandbox: playerSandbox,
    });

    const script = this.scripts.get(playerIndex) as Script;
    this.runVM(vm, script.code);

    return answer;
  }

  private runVM(vm: VM, script: string) {
    try {
      vm.run(script);
    } catch (err) {
      console.log('Error occurred while running vm script: ' + err);
    }
  }

  private convertToJson(object: any): string | null {
    try {
      return JSON.stringify(object);
    } catch (err) {
      console.log('Error occurred while converting object to JSON: ' + err);
      return null;
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
};

const provideMatchWinner = async (matchId: string, winner: number): Promise<boolean> => {
  const mutation = `
    mutation($matchId: ID!, $winner: Int!) {
      provideMatchWinner(matchId: $matchId, winner: $winner)
    }`;

  const data = await request(process.env.GRAPHQL_URL as string, mutation, { matchId, winner });

  return data.provideMatchWinner as boolean;
};
