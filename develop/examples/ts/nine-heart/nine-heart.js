var allCardSymbols = [
    "9" /* Nine */,
    "10" /* Ten */,
    "J" /* Jack */,
    "Q" /* Queen */,
    "K" /* King */,
    "As" /* As */,
];
var allCardColor = [
    0 /* Spade */,
    1 /* Trefl */,
    2 /* Caro */,
    3 /* Heart */
];
function loadInitState() {
    if (getCycle() === 0) {
        return initGame();
    }
    return loadState();
}
function takeCardsFromStack() {
    for (var i = 0; i < 3 && tableStack.length > 1; i++) {
        currentPlayerCards.push(tableStack.pop());
    }
}
function backCardToStack(indexes) {
    var lastCardSymbol;
    if ([1, 3, 4].indexOf(indexes.length) < 0) {
        throw new Error("Bad count of cards ! " + indexes.length);
    }
    for (var _i = 0, indexes_1 = indexes; _i < indexes_1.length; _i++) {
        var index = indexes_1[_i];
        var newCard = currentPlayerCards[index];
        if (newCard == null) {
            throw new Error("Unknown card ! " + index + " " + currentPlayerCards);
        }
        else if (!checkCartIsCorect(newCard)) {
            throw new Error("Bad card ! " + JSON.stringify(topOfStackCard()) + " " + JSON.stringify(newCard));
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
        var newCard = currentPlayerCards.splice(index, 1)[0];
        tableStack.push(newCard);
    }
}
function checkCartIsCorect(card) {
    var stackTop = topOfStackCard();
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
        case "As" /* As */:
            result = result || card.symbol == "As" /* As */;
    }
    return result;
}
function initGame() {
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
        playerIndex: startPlayerIndex,
        playersCards: cardsPerPlayer,
        stack: [topOfStack]
    };
}
function callPlayerAction() {
    var result = runPlayerScript(currentPlayerIndex, {
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
        case 1 /* Give */:
            backCardToStack(result.indexes);
            break;
        case 0 /* Take */:
            takeCardsFromStack();
            break;
        default:
            throw new Error("Unknown action " + result);
    }
}
function isWin() {
    return currentPlayerCards.length === 0;
}
function nextTour() {
    if (topOfStackCard().color === 0 /* Spade */) {
        state.playerIndex = state.playerIndex - 1;
        if (state.playerIndex < 0) {
            state.playerIndex = countOfPlayers - 1;
        }
    }
    else {
        state.playerIndex = (state.playerIndex + 1) % countOfPlayers;
    }
    emitFrame({
        playersCards: state.playersCards,
        stack: state.stack
    });
    saveState(state);
}
function topOfStackCard() {
    return tableStack[tableStack.length - 1];
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
// Game moc
var countOfPlayers = 2;
var state = loadInitState();
// const state: GameState = initGame();
var tableStack = state.stack;
var currentPlayerIndex = state.playerIndex;
var currentPlayerCards = state.playersCards[currentPlayerIndex];
callPlayerAction();
if (isWin()) {
    end(currentPlayerIndex);
}
else {
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
