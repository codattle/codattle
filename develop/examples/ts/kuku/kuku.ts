declare function emitFrame(frame: GameFrame): any;
declare function end(newWinderIndex: number): void;
declare function runPlayerScript(playerIndx: number, gameState: Card[]): number;
declare function getCountOfPlayers(): number;

const enum CardSymbol {
    Two = '2',
    Three = '3',
    Four = '4',
    Five = '5',
    Six = '6',
    Seven = '7',
    Eight = '8',
    Nine = '9',
    Ten = '10',
    Jack = 'J',
    Queen = 'Q',
    King = 'K',
    As = 'As',
}

const enum CardColor {
    Spade = '♠',
    Trefl = '♣',
    Caro = '♦',
    Heart = '♥',
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

interface GameFrame {
    mode: 'text';
    output: string;
}

const allCardSymbols = [
    CardSymbol.Two,
    CardSymbol.Three,
    CardSymbol.Four,
    CardSymbol.Five,
    CardSymbol.Six,
    CardSymbol.Seven,
    CardSymbol.Eight,
    CardSymbol.Nine,
    CardSymbol.Ten,
    CardSymbol.Jack,
    CardSymbol.Queen,
    CardSymbol.King,
    CardSymbol.As
];

const allCardColor = [
    CardColor.Spade,
    CardColor.Trefl,
    CardColor.Caro,
    CardColor.Heart
];

function emitText(text: string): void {
    return emitFrame({
        mode: 'text',
        output: text
    });
}

function printCard(card: Card): string {
    return card.symbol + card.color;
}

function generateCards(): Card[] {
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

function shuffle<T>(array: T[]): T[] {
    for (let counter = array.length - 1; counter > 0; counter--) {
        const index = Math.floor(Math.random() * counter);

        const temp = array[counter];
        array[counter] = array[index];
        array[index] = temp;
    }
    return array;
}

function isWin(cards: Card[]): boolean {
    return cards.every(card => card.color === cards[0].color) || cards.every(card => card.symbol === cards[0].symbol);
}

const countOfPlayers = getCountOfPlayers();
const allCards = shuffle(generateCards());

const playersCards: Card[][] = [];
for (let i = 0; i < countOfPlayers; i++) {
    if (i === 0) {
        playersCards[i] = allCards.splice(0, 4);
    } else {
        playersCards[i] = allCards.splice(0, 3);
    }
}

let playerIndex = 0;
while (true) {
    const playerCards = playersCards[playerIndex];

    emitText(playersCards.map((cards, player) => `Player ${player}: ${cards.map(card => printCard(card)).join(', ')}`).join(' '));

    if (isWin(playerCards)) {
        emitText(`Player ${playerIndex} says Kuku !!!`);
        end(playerIndex);
        break;
    }

    const indexOfcardToGiveAway = runPlayerScript(playerIndex, playerCards);
    const [cardToGiveAway] = playerCards.splice(indexOfcardToGiveAway, 1);

    if (cardToGiveAway == null) {
        throw new Error("Bad card !");
    }

    const nextPlayerIndex = (playerIndex + 1) % countOfPlayers;

    emitText(`Player ${playerIndex} gives ${printCard(cardToGiveAway)} to player ${nextPlayerIndex}`);

    playerIndex = nextPlayerIndex;
    playersCards[playerIndex].push(cardToGiveAway);
}