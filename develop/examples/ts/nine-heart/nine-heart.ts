declare function getCycle() : number;
declare function saveState(state: GameState);
declare function loadState() : GameState;
declare function emitFrame(object: GameFrame) : void;
declare function runPlayerScript(index: number, playerState: PlayerState) : PlayerScriptState;
declare function end(index: number): void;

const enum CardSymbol {
    Nine = '9',
    Ten = '10',
    Jack = 'J',
    Queen = 'Q',
    King = 'K',
    As = 'As',
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

type PlayerScriptState = {action: PlayerAction.Take} | {action: PlayerAction.Give, indexes: number[]};

interface Card {
    symbol: CardSymbol,
    color: CardColor,
}

interface GameState {
    stack: Card[];
    playersCards: Card[][];
    playerIndex: number;
}

interface PlayerState {
    stack: Card[];
    playerCards: Card[];
}

interface GameFrame {
    stack: Card[];
    playersCards: Card[][];
}

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
    

function loadInitState() : GameState {
    if (getCycle() === 0) {
        return initGame();
    }
    return loadState();
}

function takeCardsFromStack() {
    for (let i = 0; i < 3 && tableStack.length > 1; i++) {
        currentPlayerCards.push(tableStack.pop());
    }
}

function backCardToStack(indexes: number[]): void {
    let lastCardSymbol : CardSymbol;

    if ([1, 3, 4].indexOf(indexes.length) < 0) {
        throw new Error(`Bad count of cards ! ${indexes.length}`);
    }

    for (const index of indexes) {
        const newCard = currentPlayerCards[index];

        if (newCard == null) {
            throw new Error(`Unknown card ! ${index} ${currentPlayerCards}`);
        } else if (!checkCartIsCorect(newCard)) {
            throw new Error(`Bad card ! ${JSON.stringify(topOfStackCard())} ${JSON.stringify(newCard)}`);
        }

        if (lastCardSymbol == null) {
            lastCardSymbol = newCard.symbol;
        } else if (lastCardSymbol != newCard.symbol) {
            throw new Error(`All card should have this same symbol ! ${lastCardSymbol} ${newCard.symbol}`);
        }
    } 

    for (const index of indexes.sort((a,b) => b - a)) {
        const [newCard] = currentPlayerCards.splice(index, 1);
        tableStack.push(newCard);
    }
}

function checkCartIsCorect(card: Card) : boolean {
    const stackTop = topOfStackCard();

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

function initGame() : GameState {
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
        playerIndex: startPlayerIndex,
        playersCards: cardsPerPlayer,
        stack: [topOfStack]
    }
}

function callPlayerAction() {
    const result = runPlayerScript(currentPlayerIndex, {
        playerCards: currentPlayerCards,
        stack: tableStack
    });

    if (result == null) {
        throw new Error('Result is null ' + currentPlayerIndex + ' ' + JSON.stringify({
            playerCards: currentPlayerCards,
            stack: tableStack
        }));
    }

    switch (result.action) {
        case PlayerAction.Give:
            backCardToStack(result.indexes);
            break;
        case PlayerAction.Take:
            takeCardsFromStack();
            break;
        default:
            throw new Error(`Unknown action ${result}`);
    }
}

function isWin(): boolean {
    return currentPlayerCards.length === 0;
}

function nextTour() : void {
    if (topOfStackCard().color === CardColor.Spade) {
        state.playerIndex = state.playerIndex - 1;
        if (state.playerIndex < 0) {
            state.playerIndex = countOfPlayers - 1;
        }
    } else {
        state.playerIndex = (state.playerIndex + 1) % countOfPlayers;
    }

    emitFrame({
        playersCards: state.playersCards,
        stack: state.stack,
    });
    saveState(state);
}

function topOfStackCard() : Card {
    return tableStack[tableStack.length - 1];
}


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

// Game moc

const countOfPlayers : number = 2;
const state: GameState = loadInitState();
// const state: GameState = initGame();

const tableStack: Card[] = state.stack;
const currentPlayerIndex : number = state.playerIndex;
const currentPlayerCards : Card[] = state.playersCards[currentPlayerIndex];

callPlayerAction();

if (isWin()) {
    end(currentPlayerIndex);
} else {
    nextTour();
}


// Mock

// console.log('TableStack:', tableStack);
// console.log('currentPlayerIndex:', currentPlayerIndex);
// console.log('currentPlayerCards:', currentPlayerCards);

// backCardToStack(0);

// console.log('TableStack:', tableStack);
// console.log('currentPlayerIndex:', currentPlayerIndex);
// console.log('currentPlayerCards:', currentPlayerCards);
