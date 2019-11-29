var allCardSymbols = [
    "2" /* Two */,
    "3" /* Three */,
    "4" /* Four */,
    "5" /* Five */,
    "6" /* Six */,
    "7" /* Seven */,
    "8" /* Eight */,
    "9" /* Nine */,
    "10" /* Ten */,
    "J" /* Jack */,
    "Q" /* Queen */,
    "K" /* King */,
    "As" /* As */
];
var allCardColor = [
    "\u2660" /* Spade */,
    "\u2663" /* Trefl */,
    "\u2666" /* Caro */,
    "\u2665" /* Heart */
];
function emitText(text) {
    return emitFrame({
        mode: 'text',
        output: text
    });
}
function printCard(card) {
    return card.symbol + card.color;
}
function generateCards() {
    var cards = [];
    for (var _i = 0, allCardSymbols_1 = allCardSymbols; _i < allCardSymbols_1.length; _i++) {
        var symbol = allCardSymbols_1[_i];
        for (var _a = 0, allCardColor_1 = allCardColor; _a < allCardColor_1.length; _a++) {
            var color = allCardColor_1[_a];
            cards.push({
                color: color,
                symbol: symbol
            });
        }
    }
    return cards;
}
function shuffle(array) {
    for (var counter = array.length - 1; counter > 0; counter--) {
        var index = Math.floor(Math.random() * counter);
        var temp = array[counter];
        array[counter] = array[index];
        array[index] = temp;
    }
    return array;
}
function isWin(cards) {
    return cards.every(function (card) { return card.color === cards[0].color; }) || cards.every(function (card) { return card.symbol === cards[0].symbol; });
}
var countOfPlayers = getCountOfPlayers();
var allCards = shuffle(generateCards());
var playersCards = [];
for (var i = 0; i < countOfPlayers; i++) {
    if (i === 0) {
        playersCards[i] = allCards.splice(0, 4);
    }
    else {
        playersCards[i] = allCards.splice(0, 3);
    }
}
var playerIndex = 0;
while (true) {
    var playerCards = playersCards[playerIndex];
    emitText(playersCards.map(function (cards, player) { return "Player " + player + ": " + cards.map(function (card) { return printCard(card); }).join(', '); }).join(' '));
    if (isWin(playerCards)) {
        emitText("Player " + playerIndex + " says Kuku !!!");
        end(playerIndex);
        break;
    }
    var indexOfcardToGiveAway = runPlayerScript(playerIndex, playerCards);
    var cardToGiveAway = playerCards.splice(indexOfcardToGiveAway, 1)[0];
    if (cardToGiveAway == null) {
        throw new Error("Bad card !");
    }
    var nextPlayerIndex = (playerIndex + 1) % countOfPlayers;
    emitText("Player " + playerIndex + " gives " + printCard(cardToGiveAway) + " to player " + nextPlayerIndex);
    playerIndex = nextPlayerIndex;
    playersCards[playerIndex].push(cardToGiveAway);
}
