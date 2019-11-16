var allCardSymbols = [
    "9" /* Nine */,
    "10" /* Ten */,
    "J" /* Jack */,
    "Q" /* Queen */,
    "K" /* King */,
    "AS" /* As */,
];
var allCardColor = [
    0 /* Spade */,
    1 /* Trefl */,
    2 /* Caro */,
    3 /* Heart */
];
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
function initGame(countOfPlayers) {
    var allCards = shuffle(generateCards());
    var countOfAllCards = allCards.length;
    var cardsPerPlayer = [];
    var startPlayerIndex;
    var topOfStack;
    for (var i = 0; i < countOfAllCards; i++) {
        var playerIndex = i % countOfPlayers;
        var newCard = allCards.pop();
        cardsPerPlayer[playerIndex] = cardsPerPlayer[playerIndex] || [];
        if (newCard.color === 3 /* Heart */ && newCard.symbol === "9" /* Nine */) {
            startPlayerIndex = (playerIndex + 1) % countOfPlayers;
            topOfStack = newCard;
        }
        else {
            cardsPerPlayer[playerIndex].push(newCard);
        }
    }
    return {
        currentPlayerIndex: startPlayerIndex,
        playersCards: cardsPerPlayer,
        stack: [topOfStack]
    };
}
function checkCartIsCorect(stackTop, card) {
    var result = false;
    switch (stackTop.symbol) {
        case "9" /* Nine */:
            result = true;
        case "10" /* Ten */:
            result = result || card.symbol == "10" /* Ten */;
        case "J" /* Jack */:
            result = result || card.symbol == "J" /* Jack */;
        case "Q" /* Queen */:
            result = result || card.symbol == "Q" /* Queen */;
        case "K" /* King */:
            result = result || card.symbol == "K" /* King */;
        case "AS" /* As */:
            result = result || card.symbol == "AS" /* As */;
    }
    return result;
}
function takeCardsFromStack(tableStack, playerCards) {
    for (var i = 0; i < 3 && tableStack.length > 1; i++) {
        playerCards.push(tableStack.pop());
    }
}
function backCardToStack(tableStack, playerCards, indexes) {
    var lastCardSymbol;
    if ([1, 3, 4].indexOf(indexes.length) < 0) {
        throw new Error("Bad count of cards ! " + indexes.length);
    }
    for (var _i = 0, indexes_1 = indexes; _i < indexes_1.length; _i++) {
        var index = indexes_1[_i];
        var newCard = playerCards[index];
        var stackTop = tableStack[tableStack.length - 1];
        if (newCard == null) {
            throw new Error("Unknown card ! " + index + " " + playerCards);
        }
        else if (!checkCartIsCorect(stackTop, newCard)) {
            throw new Error("Bad card ! " + JSON.stringify(stackTop) + " " + JSON.stringify(newCard));
        }
        if (lastCardSymbol == null) {
            lastCardSymbol = newCard.symbol;
        }
        else if (lastCardSymbol != newCard.symbol) {
            throw new Error("All card should have this same symbol ! " + lastCardSymbol + " " + newCard.symbol);
        }
    }
    for (var _a = 0, _b = indexes.sort(function (a, b) { return b - a; }); _a < _b.length; _a++) {
        var index = _b[_a];
        var newCard = playerCards.splice(index, 1)[0];
        tableStack.push(newCard);
    }
}
//
function callPlayerAction(tableStack, playerIndex, playerCards) {
    var result = runPlayerScript(playerIndex, {
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
        case 1 /* Give */:
            backCardToStack(tableStack, playerCards, result.indexes);
            break;
        case 0 /* Take */:
            takeCardsFromStack(tableStack, playerCards);
            break;
        default:
            throw new Error("Unknown action " + result);
    }
}
// --- Game
var countOfPlayers = 3;
var state = initGame(countOfPlayers);
var tableStack = state.stack;
var currentPlayerIndex = state.currentPlayerIndex;
while (true) {
    var currentPlayerCards = state.playersCards[currentPlayerIndex];
    callPlayerAction(tableStack, currentPlayerIndex, currentPlayerCards);
    var topOfStack = tableStack[tableStack.length - 1];
    emitFrame({
        mode: 'text',
        output: JSON.stringify({ top: topOfStack, cards: state.playersCards })
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
