import { ObjectID } from 'bson';
import { VM } from 'vm2';
import * as dao from './dao';
import { Game } from './model/game';
import { Match } from './model/match';
import { IScript, Script } from './model/script';

const MAX_CYCLES = 100;

const runPlayerScript = (script: IScript) => {
  let answer: any;
  const vm = new VM({
    timeout: 1000,
    sandbox: {
      answer: (playerAnswer: any) => (answer = playerAnswer),
    },
  });
  vm.run(script.content);
  return answer;
};

export const executeSimulation = async (matchId: ObjectID) => {
  const match = await dao.findById(matchId, Match);
  const game = await dao.findById(match.game, Game);
  const scripts = await dao.findByIds(match.scripts, Script);

  let gameData: any = null;
  let framesToEmit: any[] = [];
  let frameOrder = 0;
  let winner = null;
  let isGameEnd = false;
  let cycle = 0;

  const sandbox = {
    loadData: () => gameData,
    saveData: (data: any) => (gameData = data),
    emitFrame: (frame: any) => framesToEmit.push(frame),
    end: (newWinner: number | null) => {
      winner = newWinner;
      isGameEnd = true;
    },
    runPlayerScript: (playerIndex: number) => runPlayerScript(scripts[playerIndex]),
    getCycle: () => cycle,
  };

  match.result = {
    winner: null,
    resultFrames: [],
  };

  const vm = new VM({
    timeout: 10000,
    sandbox,
  });
  while (!isGameEnd && cycle < MAX_CYCLES) {
    vm.run(game.mainLoop);
    const newFrames = framesToEmit.map((frameToEmit: any) => ({ order: frameOrder++, content: frameToEmit }));
    match.result.resultFrames.push(...newFrames);
    await dao.save(match);

    cycle++;
    framesToEmit = [];
  }

  match.result.winner = winner;
  await dao.save(match);
};
