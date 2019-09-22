function parseChoice(choice) {
  return ["ROCK", "PAPER", "SCISSORS"].includes(choice) ? choice : "NO_CHOICE";
}

function isWin(firstChoice, secondChoice) {
  return (
    (firstChoice !== "NO_CHOICE" && secondChoice === "NO_CHOICE") ||
    (firstChoice === "ROCK" && secondChoice === "SCISSORS") ||
    (firstChoice === "PAPER" && secondChoice === "ROCK") ||
    (firstChoice === "SCISSORS" && secondChoice === "PAPER")
  );
}

var state = {
  score1: 0,
  score2: 0
};

while (state.score1 < 3 && state.score2 < 3) {
  var firstPlayerChoice = parseChoice(runPlayerScript(0));
  var secondPlayerChoice = parseChoice(runPlayerScript(1));

  if (isWin(firstPlayerChoice, secondPlayerChoice)) {
    state.score1++;
  } else if (isWin(secondPlayerChoice, firstPlayerChoice)) {
    state.score2++;
  }

  emitFrame({
    mode: "text",
    output: firstPlayerChoice + " vs " + secondPlayerChoice
  });
}

if (state.score1 === 3) {
  end(0);
} else if (state.score2 === 3) {
  end(1);
}
