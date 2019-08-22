// WORK IN PROGRESS

const HEADQUARTERS = "HEADQUARTERS";
const WORKER = "WORKER";
const WARRIOR = "WARRIOR";
const TREE = "TREE";
const STONE = "STONE";
const IRON = "IRON";

const PLAYER_COUNT = 2;
const WIDTH = 41;
const HEIGHT = 41;

const createHeadquarters = (x, y, player) => ({ type: HEADQUARTERS, x, y, player, hp: 100 });
const createWorker = (x, y, player) => ({ type: WORKER, x, y, player, hp: 10 });
const createWarrior = (x, y, player) => ({ type: WARRIOR, x, y, player, hp: 20 });
const createTree = (x, y) => ({ type: TREE, x, y, resources: 100, resourcesType: "wood" });
const createStone = (x, y) => ({ type: STONE, x, y, resources: 100, resourcesType: "stone" });
const createIron = (x, y) => ({ type: IRON, x, y, resources: 100, resourcesType: "iron" });

const deepCopy = x => JSON.parse(JSON.stringify(x));

const initState = () => {
  const getPlayerState = () => {
    return {
      wood: 0,
      stone: 0,
      iron: 0
    };
  };

  const generateMap = () => {
    const middleX = WIDTH / 2;
    const middleY = HEIGHT / 2;

    return [
      createHeadquarters(2, middleY, 0),
      createWorker(1, middleY, 0),
      createWorker(1, middleY - 1, 0),
      createWorker(1, middleY + 1, 0),
      createWarrior(3, middleY, 0),
      createTree(middleX, middleY - 10),
      createTree(middleX, middleY + 10),
      createTree(middleX - 3, middleY + 5),
      createTree(middleX + 3, middleY - 5),
      createTree(middleX - 7, middleY + 6),
      createTree(middleX + 7, middleY - 6),
      createTree(middleX - 12, middleY + 8),
      createTree(middleX + 12, middleY - 8),
      createStone(middleX + 5, middleY + 5),
      createStone(middleX - 5, middleY - 5),
      createStone(middleX + 2, middleY + 6),
      createStone(middleX - 2, middleY - 6),
      createStone(middleX + 11, middleY + 4),
      createStone(middleX - 11, middleY - 4),
      createIron(middleX, middleY),
      createIron(middleX + 6, middleY - 6),
      createIron(middleX - 6, middleY + 6),
      createHeadquarters(WIDTH - 3, middleY, 1),
      createWorker(WIDTH - 2, middleY, 1),
      createWorker(WIDTH - 2, middleY - 1, 1),
      createWorker(WIDTH - 2, middleY + 1, 1),
      createWarrior(WIDTH - 4, middleY, 1)
    ];
  };

  return {
    players: [...Array(PLAYER_COUNT).keys()].map(i => getPlayerState()),
    map: generateMap()
  };
};

const validatePosition = position => position && Number.isInteger(x) && Number.isInteger(y) && x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
const validateMovementDistance = (from, to) => Math.abs(from.x - to.x) <= 1 && Math.abs(from.y - to.y) <= 1;

let state = getCycle() === 0 ? initState() : loadState();

const getObject = (x, y) => state.map.find(object => object.x === x && object.y === y);
const removeObject = (x, y) => {
  state.map = state.map.filter(object => object.x !== x && object.y !== y);
};

const collectResources = (x, y, player) => {
  const object = getObject(x, y);
  if (object.resources >= 10) {
    object.resources -= 10;
    state.players[player][object.resourcesType] += 10;
  } else {
    state.players[player][object.resourcesType] += object.resources;
    removeObject(x, y);
  }
};

[...Array(PLAYER_COUNT).keys()].forEach(player => {
  const answer = runPlayerScript(player, deepCopy({ player: state.players[player], map: state.map }));
  if (!Array.isArray(answer)) {
    return;
  }
  answer.forEach(movement => {
    if (!movement) {
      return;
    }
    if (movement.type === "MOVE") {
      if (!validatePosition(movement.from) || !validatePosition(movement.to) || !validateMovementDistance(movement.from, movement.to)) {
        return;
      }
      const object = getObject(movement.from.x, movement.from.y);
      if (!object || object.player !== player) {
        return;
      }
      if (object.type === WORKER) {
        const target = getObject(movement.to.x, movement.to.y);
        if (target === undefined) {
          object.moved = true;
          object.x = movement.to.x;
          object.y = movement.to.y;
        } else if (target.type === TREE || target.type === STONE || target.type === IRON) {
          object.moved = true;
          collectResources(target.x, target.y, player);
        }
      }
    }
  });
});

const objects = state.map.map(object => ({
  x: object.x * 20,
  y: object.y * 20,
  width: 20,
  height: 20,
  sprite: object.type + (object.player || "")
}));

saveState(state);
emitFrame({ mode: "2d", width: WIDTH * 20, height: HEIGHT * 20, objects });
end(0);
