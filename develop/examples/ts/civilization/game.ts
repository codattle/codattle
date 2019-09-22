// WORK IN PROGRESS

import { Component, Engine, Family, FamilyBuilder, System, Entity, ComponentClass } from "@nova-engine/ecs";

const PLAYER_COUNT = 2;
const WIDTH = 41;
const HEIGHT = 41;

type Resources = { [resource: string]: number };

class Player {
  id: number;
  resources: Resources;
  constructor(id: number, resources: Resources) {
    this.id = id;
    this.resources = resources;
  }
}

interface GameObjectPlayerView {
  type: GameObjectType;
  player: number | undefined;
  x: number;
  y: number;
}

class PlayerState {
  player: Player;
  objects: GameObjectPlayerView[];
  constructor(player: Player, objects: GameObjectPlayerView[]) {
    this.player = player;
    this.objects = objects;
  }
}

class GameFrame {}

declare function emitFrame(object: GameFrame): void;
declare function runPlayerScript(index: number, playerState: PlayerState): any;
declare function end(index: number): void;

enum GameObjectType {
  Headquarters,
  Worker,
  Warrior,
  Tree,
  Stone,
  Iron
}

enum ResourceType {
  Tree,
  Stone,
  Iron
}

class IdGenerator {
  private static id: number = 0;
  static next(): number {
    return this.id++;
  }
}

class Position {
  x: number;
  y: number;

  constructor(x: number = 0, y: number = 0) {
    this.x = x;
    this.y = y;
  }

  equals(other: Position): boolean {
    return this.x === other.x && this.y === other.y;
  }
}

class HasHpComponent implements Component {
  static readonly Class = (HasHpComponent as any) as ComponentClass<HasHpComponent>;
  hp: number = 100;
}

class HasPlayerComponent implements Component {
  static readonly Class = (HasPlayerComponent as any) as ComponentClass<HasPlayerComponent>;
  player: number = 0;
}

class CanMoveComponent implements Component {
  static readonly Class = (CanMoveComponent as any) as ComponentClass<CanMoveComponent>;
  alreadyMoved: boolean = false;
}

class HasResourceComponent implements Component {
  static readonly Class = (HasResourceComponent as any) as ComponentClass<HasResourceComponent>;
  resourceType: ResourceType = ResourceType.Tree;
  resources: number = 100;
}

class CanCollectResourceComponent implements Component {
  static readonly Class = (CanCollectResourceComponent as any) as ComponentClass<CanCollectResourceComponent>;
}

class CanAttackResourceComponent implements Component {
  static readonly Class = (CanAttackResourceComponent as any) as ComponentClass<CanAttackResourceComponent>;
  damage: number = 5;
}

abstract class GameObject extends Entity {
  position: Position;
  constructor(position: Position) {
    super();
    this.id = IdGenerator.next();
    this.position = position;
  }
  abstract getType(): GameObjectType;
}

class Headquarters extends GameObject {
  constructor(position: Position, player: number) {
    super(position);
    this.putComponent(HasPlayerComponent.Class);
    this.putComponent(HasHpComponent.Class);

    this.getComponent(HasPlayerComponent.Class).player = player;
    this.getComponent(HasHpComponent.Class).hp = 100;
  }
  getType(): GameObjectType {
    return GameObjectType.Headquarters;
  }
}

class Worker extends GameObject {
  constructor(position: Position, player: number) {
    super(position);
    this.putComponent(HasPlayerComponent.Class);
    this.putComponent(HasHpComponent.Class);
    this.putComponent(CanMoveComponent.Class);
    this.putComponent(CanCollectResourceComponent.Class);

    this.getComponent(HasPlayerComponent.Class).player = player;
    this.getComponent(HasHpComponent.Class).hp = 10;
  }
  getType(): GameObjectType {
    return GameObjectType.Worker;
  }
}

class Warrior extends GameObject {
  constructor(position: Position, player: number) {
    super(position);
    this.putComponent(HasPlayerComponent.Class);
    this.putComponent(HasHpComponent.Class);
    this.putComponent(CanMoveComponent.Class);
    this.putComponent(CanAttackResourceComponent.Class);

    this.getComponent(HasPlayerComponent.Class).player = player;
    this.getComponent(HasHpComponent.Class).hp = 20;
  }
  getType(): GameObjectType {
    return GameObjectType.Warrior;
  }
}

class Tree extends GameObject {
  constructor(position: Position) {
    super(position);
    this.putComponent(HasResourceComponent.Class);

    this.getComponent(HasResourceComponent.Class).resourceType = ResourceType.Tree;
  }
  getType(): GameObjectType {
    return GameObjectType.Tree;
  }
}

class Stone extends GameObject {
  constructor(position: Position) {
    super(position);
    this.putComponent(HasResourceComponent.Class);

    this.getComponent(HasResourceComponent.Class).resourceType = ResourceType.Stone;
  }
  getType(): GameObjectType {
    return GameObjectType.Stone;
  }
}

class Iron extends GameObject {
  constructor(position: Position) {
    super(position);
    this.putComponent(HasResourceComponent.Class);

    this.getComponent(HasResourceComponent.Class).resourceType = ResourceType.Iron;
  }
  getType(): GameObjectType {
    return GameObjectType.Iron;
  }
}

function deepCopy<T>(x: T): T {
  return JSON.parse(JSON.stringify(x));
}

function preparePlayer(id: number): Player {
  const resources: Resources = {};
  resources[ResourceType.Tree] = 0;
  resources[ResourceType.Stone] = 0;
  resources[ResourceType.Iron] = 0;
  return { id, resources };
}

function preparePlayers(): Player[] {
  return [...Array(PLAYER_COUNT).keys()].map(i => preparePlayer(i));
}

function prepareObjects(): GameObject[] {
  const middleX = Math.floor(WIDTH / 2);
  const middleY = Math.floor(HEIGHT / 2);

  return [
    new Headquarters(new Position(2, middleY), 0),
    new Worker(new Position(1, middleY), 0),
    new Worker(new Position(1, middleY + 1), 0),
    new Worker(new Position(1, middleY - 1), 0),
    new Warrior(new Position(3, middleY - 1), 0),
    new Tree(new Position(middleX, middleY - 1)),
    new Stone(new Position(middleX, middleY - 3)),
    new Iron(new Position(middleX, middleY + 2)),
    new Headquarters(new Position(WIDTH - 3, middleY), 1),
    new Worker(new Position(WIDTH - 2, middleY), 1),
    new Worker(new Position(WIDTH - 2, middleY + 1), 1),
    new Worker(new Position(WIDTH - 2, middleY - 1), 1),
    new Warrior(new Position(WIDTH - 4, middleY - 1), 1)
  ];
}

function castToPosition(position: any): Position | null {
  if (!position) {
    return null;
  }
  const { x, y } = position;
  if (Number.isInteger(x) && Number.isInteger(y) && x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
    return new Position(x, y);
  } else {
    return null;
  }
}

function validateMovementDistance(from: Position, to: Position): boolean {
  return Math.abs(from.x - to.x) <= 1 && Math.abs(from.y - to.y) <= 1;
}

let winner: number | undefined = undefined;

class PlayerMovementSystem extends System {
  readonly players: Player[];
  canMoveComponents?: Family;

  constructor(players: Player[]) {
    super();
    this.priority = 100;
    this.players = players;
  }

  onAttach(engine: Engine) {
    super.onAttach(engine);
    this.canMoveComponents = new FamilyBuilder(engine).include(CanMoveComponent.Class).build();
  }

  update(engine: Engine) {
    for (let entity of (this.canMoveComponents as Family).entities) {
      entity.getComponent(CanMoveComponent.Class).alreadyMoved = false;
    }

    [...Array(PLAYER_COUNT).keys()].forEach(player => {
      const objectsPlayerView = (engine.entities as GameObject[]).map(object => ({
        type: object.getType(),
        player: object.hasComponent(HasPlayerComponent.Class) ? object.getComponent(HasPlayerComponent.Class).player : undefined,
        x: object.position.x,
        y: object.position.y
      }));
      const answer = runPlayerScript(player, deepCopy({ player: this.players[player], objects: objectsPlayerView }));
      if (!Array.isArray(answer)) {
        return;
      }
      answer.forEach(movement => {
        if (!movement) {
          return;
        }
        if (movement.type === "MOVE") {
          const [from, to] = [castToPosition(movement.from), castToPosition(movement.to)];
          if (!from || !to || !validateMovementDistance(from, to)) {
            return;
          }
          const object = this.getGameObjectByPosition(engine, from);
          if (
            !object ||
            !object.hasComponent(HasPlayerComponent.Class) ||
            object.getComponent(HasPlayerComponent.Class).player !== player ||
            !object.hasComponent(CanMoveComponent.Class) ||
            object.getComponent(CanMoveComponent.Class).alreadyMoved
          ) {
            return;
          }
          const destinationObject = this.getGameObjectByPosition(engine, to);
          if (destinationObject === undefined) {
            object.position = to;
            object.getComponent(CanMoveComponent.Class).alreadyMoved = true;
          } else if (destinationObject.hasComponent(HasResourceComponent.Class)) {
            if (object.hasComponent(CanCollectResourceComponent.Class)) {
              const hasResourceComponent = destinationObject.getComponent(HasResourceComponent.Class);
              this.players[player].resources[hasResourceComponent.resourceType]++;
              hasResourceComponent.resources--;
              if (hasResourceComponent.resources <= 0) {
                engine.removeEntity(destinationObject);
              }
            }
          } else if (destinationObject.hasComponent(HasHpComponent.Class)) {
            if (object.hasComponent(CanAttackResourceComponent.Class)) {
              destinationObject.getComponent(HasHpComponent.Class).hp -= object.getComponent(CanAttackResourceComponent.Class).damage;
              if (destinationObject.getComponent(HasHpComponent.Class).hp <= 0) {
                engine.removeEntity(destinationObject);
              }
            }
          }
        }
      });
    });
  }

  private getGameObjectByPosition(engine: Engine, position: Position): GameObject | undefined {
    return (engine.entities as GameObject[]).find(object => object.position.equals(position));
  }
}

class WinSystem extends System {
  constructor() {
    super();
    this.priority = 200;
  }

  update(engine: Engine) {
    const hasPlayerHeadquarters = [...Array(PLAYER_COUNT).keys()].map(player =>
      engine.entities.some(object => object instanceof Headquarters && object.getComponent(HasPlayerComponent.Class).player === player)
    );
    if (hasPlayerHeadquarters.filter(x => x).length === 1) {
      winner = hasPlayerHeadquarters.indexOf(true);
    }
  }
}

const players = preparePlayers();

const engine = new Engine();
engine.addSystem(new PlayerMovementSystem(players));
engine.addSystem(new WinSystem());
engine.addEntities(...prepareObjects());

while (winner === undefined) {
  engine.update(0);

  const objects = (engine.entities as GameObject[]).map(object => ({
    x: object.position.x * 20,
    y: object.position.y * 20,
    width: 20,
    height: 20,
    sprite:
      GameObjectType[object.getType()] +
      ((object.hasComponent(HasPlayerComponent.Class) ? object.getComponent(HasPlayerComponent.Class).player : ""))
  }));

  emitFrame({ mode: "2d", width: WIDTH * 20, height: HEIGHT * 20, objects });
}

end(winner);
