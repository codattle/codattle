function initState() {
    return {
        value: Math.floor(Math.random() * 100),
        points: [0, 0]
    };
}

var state = getCycle() === 0 ? initState() : loadState();

var answers = [
    Number(runPlayerScript(0)), 
    Number(runPlayerScript(1))
];

var processedAnswers = [
    Math.abs(state.value - answers[0]),
    Math.abs(state.value - answers[1])
];

emitFrame({
   answers: answers,
   values: state.value,
   points: state.points
});

if (processedAnswers[0] < processedAnswers[1]) {
    state.points[0]++;
    state.points[1]--;
} else if (state.points[0] > state.points[1]) {
    state.points[0]--;
    state.points[1]++;
}
saveState(initState());

if (getCycle() === 4) {
    if (state.points[0] >= state.points[1]) {
        end(0);
    } else if (state.points[0] < state.points[1]) {
        end(1);
    }
}

