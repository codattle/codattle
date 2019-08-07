
var cardPower = ['9', '10', 'J', 'Q', 'K', 'As'];

function compareCards(a, b) {
   return cardPower.indexOf(a.symbol) - cardPower.indexOf(b.symbol);
}

var state = getGameState();
var stack = state.stack;
var cards = state.playerCards;

var stackTop = stack[stack.length - 1];

var selectedCard;
for (var cardIndex in cards) {
    var card = cards[cardIndex];
    if (compareCards(card, stackTop) >= 0) {
        if (selectedCard == null || compareCards(card, selectedCard) < 0) {
            selectedCard = card;
        }
    }
}

var selectedCardsIndexes = [];
if (selectedCard != null) {
    for (var cardIndex in cards) {
        var card = cards[cardIndex];
        if (card.symbol === selectedCard.symbol) {
            selectedCardsIndexes.push(Number(cardIndex));
        }
    }
}

if (selectedCardsIndexes.length === 2) {
    selectedCardsIndexes.pop();
}

if (selectedCardsIndexes.length > 0) {
    answer({
        action: 1,
        indexes: selectedCardsIndexes
    });
} else {
    answer({
        action: 0
    });
}