// WORK IN PROGRESS

const { player, objects } = getGameState();
answer(
  objects.map(object => ({
    type: "MOVE",
    from: {
      x: object.x,
      y: object.y
    },
    to: {
      x: object.x,
      y: object.y + 1
    }
  }))
);
