
function randomIndex(array) {
    return Math.floor(Math.random() * array.length);
} 

answer(randomIndex(getGameState()));
