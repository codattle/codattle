declare function emitFrame(frame: GameFrame): any;
declare function end(newWinderIndex: number): void;
declare function runPlayerScript(playerIndx: number, gameState: PlayerState): PlayerScriptState;

const enum CardSymbol {
    Nine = '9',
    Ten = '10',
    Jack = 'J',
    Queen = 'Q',
    King = 'K',
    As = 'AS'
}

const enum CardColor {
    Spade,
    Trefl,
    Caro,
    Heart
}

const enum PlayerAction {
    Take,
    Give,
}

interface Card {
    symbol: CardSymbol,
    color: CardColor,
}

interface GameState {
    stack: Card[];
    playersCards: Card[][];
    currentPlayerIndex: number;
}

interface PlayerState {
    stack: Card[];
    playerCards: Card[];
}

interface GameFrame {
    mode: 'text';
    output: string;
}

type PlayerScriptState = {action: PlayerAction.Take} | {action: PlayerAction.Give, indexes: number[]};

const allCardSymbols = [
    CardSymbol.Nine,
    CardSymbol.Ten,
    CardSymbol.Jack,
    CardSymbol.Queen,
    CardSymbol.King,
    CardSymbol.As, 
];    

const allCardColor = [
    CardColor.Spade,
    CardColor.Trefl,
    CardColor.Caro,
    CardColor.Heart
];

function generateCards() : Card[] {
    const cards = [] as Card[];
    for (const symbol of allCardSymbols) {
        for (const color of allCardColor) {
            cards.push({
                color: color,
                symbol: symbol
            });
        }
    }
    return cards;
}

function shuffle<T>(array: T[]) : T[] {
    for (let counter = array.length - 1; counter > 0; counter --) {
        const index = Math.floor(Math.random() * counter);
        
        const temp = array[counter];
        array[counter] = array[index];
        array[index] = temp;
    }
    return array;
} 

function initGame(countOfPlayers: number) : GameState {
    const allCards = shuffle(generateCards());
    const countOfAllCards = allCards.length;

    const cardsPerPlayer = [];
    let startPlayerIndex: number;
    let topOfStack: Card;
    for (let i = 0; i < countOfAllCards; i++) {
        const playerIndex = i % countOfPlayers;
        const newCard = allCards.pop();
        cardsPerPlayer[playerIndex] = cardsPerPlayer[playerIndex] || [];
        
        if (newCard.color === CardColor.Heart && newCard.symbol === CardSymbol.Nine) {
            startPlayerIndex = (playerIndex + 1) % countOfPlayers;
            topOfStack = newCard;
        } else {
            cardsPerPlayer[playerIndex].push(newCard);
        }
    }

    return {
        currentPlayerIndex: startPlayerIndex,
        playersCards: cardsPerPlayer,
        stack: [topOfStack]
    }
}

function checkCartIsCorect(stackTop: Card, card: Card) : boolean {
    let result = false;
    switch (stackTop.symbol) {
        case CardSymbol.Nine:
            result = true
        case CardSymbol.Ten:
            result = result || card.symbol == CardSymbol.Ten;
        case CardSymbol.Jack:
            result = result || card.symbol == CardSymbol.Jack;
        case CardSymbol.Queen:
            result = result || card.symbol == CardSymbol.Queen;
        case CardSymbol.King:
            result = result || card.symbol == CardSymbol.King;
        case CardSymbol.As:
            result = result || card.symbol == CardSymbol.As;
    }
    return result;
}

function takeCardsFromStack(tableStack: Card[], playerCards: Card[]): void {
    for (let i = 0; i < 3 && tableStack.length > 1; i++) {
        playerCards.push(tableStack.pop());
    }
}

function backCardToStack(tableStack: Card[], playerCards: Card[], indexes: number[]): void {
    let lastCardSymbol : CardSymbol;

    if ([1, 3, 4].indexOf(indexes.length) < 0) {
        throw new Error(`Bad count of cards ! ${indexes.length}`);
    }

    for (const index of indexes) {
        const newCard = playerCards[index];
        const stackTop = tableStack[tableStack.length - 1];

        if (newCard == null) {
            throw new Error(`Unknown card ! ${index} ${playerCards}`);
        } else if (!checkCartIsCorect(stackTop, newCard)) {
            throw new Error(`Bad card ! ${JSON.stringify(stackTop)} ${JSON.stringify(newCard)}`);
        }

        if (lastCardSymbol == null) {
            lastCardSymbol = newCard.symbol;
        } else if (lastCardSymbol != newCard.symbol) {
            throw new Error(`All card should have this same symbol ! ${lastCardSymbol} ${newCard.symbol}`);
        }
    } 

    for (const index of indexes.sort((a,b) => b - a)) {
        const [newCard] = playerCards.splice(index, 1);
        tableStack.push(newCard);
    }
}

//


function callPlayerAction(tableStack: Card[], playerIndex: number, playerCards: Card[]) {
    const result = runPlayerScript(playerIndex, {
        playerCards: playerCards,
        stack: tableStack
    });

    if (result == null) {
        throw new Error('Result is null ' + playerIndex + ' ' + JSON.stringify({
            playerCards: playerCards,
            stack: tableStack
        }));
    }

    switch (result.action) {
        case PlayerAction.Give:
            backCardToStack(tableStack, playerCards, result.indexes);
            break;
        case PlayerAction.Take:
            takeCardsFromStack(tableStack, playerCards);
            break;
        default:
            throw new Error(`Unknown action ${result}`);
    }
}


// --- Game

const countOfPlayers = 3;
const state = initGame(countOfPlayers);

const tableStack: Card[] = state.stack;
let currentPlayerIndex : number = state.currentPlayerIndex;

while (true) {
    const currentPlayerCards : Card[] = state.playersCards[currentPlayerIndex];

    callPlayerAction(tableStack, currentPlayerIndex, currentPlayerCards);

    let topOfStack: Card = tableStack[tableStack.length - 1];

    emitFrame({
        mode: 'text',
        output: JSON.stringify({top: topOfStack, cards: state.playersCards})
    });

    if (currentPlayerCards.length === 0) {
        end(currentPlayerIndex);
        break;
    }

    // if (topOfStack.color === CardColor.Spade) {
    //     if (currentPlayerIndex === 0) {
    //         currentPlayerIndex = (countOfPlayers - 1);
    //     } else {
    //         currentPlayerIndex -= 1;
    //     }
    // } else {
        currentPlayerIndex = (currentPlayerIndex + 1) % countOfPlayers;
    // }
}