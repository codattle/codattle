"use strict";
// WORK IN PROGRESS
Object.defineProperty(exports, "__esModule", { value: true });
const ecs_1 = require("@nova-engine/ecs");
const PLAYER_COUNT = 2;
const WIDTH = 41;
const HEIGHT = 41;
class Player {
    constructor(id, resources) {
        this.id = id;
        this.resources = resources;
    }
}
class PlayerState {
    constructor(player, objects) {
        this.player = player;
        this.objects = objects;
    }
}
class GameFrame {
}
var GameObjectType;
(function (GameObjectType) {
    GameObjectType[GameObjectType["Headquarters"] = 0] = "Headquarters";
    GameObjectType[GameObjectType["Worker"] = 1] = "Worker";
    GameObjectType[GameObjectType["Warrior"] = 2] = "Warrior";
    GameObjectType[GameObjectType["Tree"] = 3] = "Tree";
    GameObjectType[GameObjectType["Stone"] = 4] = "Stone";
    GameObjectType[GameObjectType["Iron"] = 5] = "Iron";
})(GameObjectType || (GameObjectType = {}));
var ResourceType;
(function (ResourceType) {
    ResourceType[ResourceType["Tree"] = 0] = "Tree";
    ResourceType[ResourceType["Stone"] = 1] = "Stone";
    ResourceType[ResourceType["Iron"] = 2] = "Iron";
})(ResourceType || (ResourceType = {}));
class IdGenerator {
    static next() {
        return this.id++;
    }
}
IdGenerator.id = 0;
class Position {
    constructor(x = 0, y = 0) {
        this.x = x;
        this.y = y;
    }
    equals(other) {
        return this.x === other.x && this.y === other.y;
    }
}
class HasHpComponent {
    constructor() {
        this.hp = 100;
    }
}
HasHpComponent.Class = HasHpComponent;
class HasPlayerComponent {
    constructor() {
        this.player = 0;
    }
}
HasPlayerComponent.Class = HasPlayerComponent;
class CanMoveComponent {
    constructor() {
        this.alreadyMoved = false;
    }
}
CanMoveComponent.Class = CanMoveComponent;
class HasResourceComponent {
    constructor() {
        this.resourceType = ResourceType.Tree;
        this.resources = 100;
    }
}
HasResourceComponent.Class = HasResourceComponent;
class CanCollectResourceComponent {
}
CanCollectResourceComponent.Class = CanCollectResourceComponent;
class CanAttackResourceComponent {
    constructor() {
        this.damage = 5;
    }
}
CanAttackResourceComponent.Class = CanAttackResourceComponent;
class GameObject extends ecs_1.Entity {
    constructor(position) {
        super();
        this.id = IdGenerator.next();
        this.position = position;
    }
}
class Headquarters extends GameObject {
    constructor(position, player) {
        super(position);
        this.putComponent(HasPlayerComponent.Class);
        this.putComponent(HasHpComponent.Class);
        this.getComponent(HasPlayerComponent.Class).player = player;
        this.getComponent(HasHpComponent.Class).hp = 100;
    }
    getType() {
        return GameObjectType.Headquarters;
    }
}
class Worker extends GameObject {
    constructor(position, player) {
        super(position);
        this.putComponent(HasPlayerComponent.Class);
        this.putComponent(HasHpComponent.Class);
        this.putComponent(CanMoveComponent.Class);
        this.putComponent(CanCollectResourceComponent.Class);
        this.getComponent(HasPlayerComponent.Class).player = player;
        this.getComponent(HasHpComponent.Class).hp = 10;
    }
    getType() {
        return GameObjectType.Worker;
    }
}
class Warrior extends GameObject {
    constructor(position, player) {
        super(position);
        this.putComponent(HasPlayerComponent.Class);
        this.putComponent(HasHpComponent.Class);
        this.putComponent(CanMoveComponent.Class);
        this.putComponent(CanAttackResourceComponent.Class);
        this.getComponent(HasPlayerComponent.Class).player = player;
        this.getComponent(HasHpComponent.Class).hp = 20;
    }
    getType() {
        return GameObjectType.Warrior;
    }
}
class Tree extends GameObject {
    constructor(position) {
        super(position);
        this.putComponent(HasResourceComponent.Class);
        this.getComponent(HasResourceComponent.Class).resourceType = ResourceType.Tree;
    }
    getType() {
        return GameObjectType.Tree;
    }
}
class Stone extends GameObject {
    constructor(position) {
        super(position);
        this.putComponent(HasResourceComponent.Class);
        this.getComponent(HasResourceComponent.Class).resourceType = ResourceType.Stone;
    }
    getType() {
        return GameObjectType.Stone;
    }
}
class Iron extends GameObject {
    constructor(position) {
        super(position);
        this.putComponent(HasResourceComponent.Class);
        this.getComponent(HasResourceComponent.Class).resourceType = ResourceType.Iron;
    }
    getType() {
        return GameObjectType.Iron;
    }
}
function deepCopy(x) {
    return JSON.parse(JSON.stringify(x));
}
function preparePlayer(id) {
    const resources = {};
    resources[ResourceType.Tree] = 0;
    resources[ResourceType.Stone] = 0;
    resources[ResourceType.Iron] = 0;
    return { id, resources };
}
function preparePlayers() {
    return [...Array(PLAYER_COUNT).keys()].map(i => preparePlayer(i));
}
function prepareObjects() {
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
function castToPosition(position) {
    if (!position) {
        return null;
    }
    const { x, y } = position;
    if (Number.isInteger(x) && Number.isInteger(y) && x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        return new Position(x, y);
    }
    else {
        return null;
    }
}
function validateMovementDistance(from, to) {
    return Math.abs(from.x - to.x) <= 1 && Math.abs(from.y - to.y) <= 1;
}
let winner = undefined;
class PlayerMovementSystem extends ecs_1.System {
    constructor(players) {
        super();
        this.priority = 100;
        this.players = players;
    }
    onAttach(engine) {
        super.onAttach(engine);
        this.canMoveComponents = new ecs_1.FamilyBuilder(engine).include(CanMoveComponent.Class).build();
    }
    update(engine) {
        for (let entity of this.canMoveComponents.entities) {
            entity.getComponent(CanMoveComponent.Class).alreadyMoved = false;
        }
        [...Array(PLAYER_COUNT).keys()].forEach(player => {
            const objectsPlayerView = engine.entities.map(object => ({
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
                    if (!object ||
                        !object.hasComponent(HasPlayerComponent.Class) ||
                        object.getComponent(HasPlayerComponent.Class).player !== player ||
                        !object.hasComponent(CanMoveComponent.Class) ||
                        object.getComponent(CanMoveComponent.Class).alreadyMoved) {
                        return;
                    }
                    const destinationObject = this.getGameObjectByPosition(engine, to);
                    if (destinationObject === undefined) {
                        object.position = to;
                        object.getComponent(CanMoveComponent.Class).alreadyMoved = true;
                    }
                    else if (destinationObject.hasComponent(HasResourceComponent.Class)) {
                        if (object.hasComponent(CanCollectResourceComponent.Class)) {
                            const hasResourceComponent = destinationObject.getComponent(HasResourceComponent.Class);
                            this.players[player].resources[hasResourceComponent.resourceType]++;
                            hasResourceComponent.resources--;
                            if (hasResourceComponent.resources <= 0) {
                                engine.removeEntity(destinationObject);
                            }
                        }
                    }
                    else if (destinationObject.hasComponent(HasHpComponent.Class)) {
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
    getGameObjectByPosition(engine, position) {
        return engine.entities.find(object => object.position.equals(position));
    }
}
class WinSystem extends ecs_1.System {
    constructor() {
        super();
        this.priority = 200;
    }
    update(engine) {
        const hasPlayerHeadquarters = [...Array(PLAYER_COUNT).keys()].map(player => engine.entities.some(object => object instanceof Headquarters && object.getComponent(HasPlayerComponent.Class).player === player));
        if (hasPlayerHeadquarters.filter(x => x).length === 1) {
            winner = hasPlayerHeadquarters.indexOf(true);
        }
    }
}
const players = preparePlayers();
const engine = new ecs_1.Engine();
engine.addSystem(new PlayerMovementSystem(players));
engine.addSystem(new WinSystem());
engine.addEntities(...prepareObjects());
while (winner === undefined) {
    engine.update(0);
    const objects = engine.entities.map(object => ({
        x: object.position.x * 20,
        y: object.position.y * 20,
        width: 20,
        height: 20,
        sprite: GameObjectType[object.getType()] +
            ((object.hasComponent(HasPlayerComponent.Class) ? object.getComponent(HasPlayerComponent.Class).player : ""))
    }));
    emitFrame({ mode: "2d", width: WIDTH * 20, height: HEIGHT * 20, objects });
}
end(winner);
