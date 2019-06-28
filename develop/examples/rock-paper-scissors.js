function initState() {
  return {
    score1: 0,
    score2: 0
  };
}

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

var data = getCycle() === 0 ? initState() : loadState();

var firstPlayerChoice = parseChoice(runPlayerScript(0));
var secondPlayerChoice = parseChoice(runPlayerScript(1));

emitFrame({
  first: firstPlayerChoice,
  second: secondPlayerChoice
});

if (isWin(firstPlayerChoice, secondPlayerChoice)) {
  data.score1++;
} else if (isWin(secondPlayerChoice, firstPlayerChoice)) {
  data.score2++;
}

saveState(data);

if (data.score1 === 3) {
  end(0);
} else if (data.score2 === 3) {
  end(1);
} else if (getCycle() === 10) {
  end(null);
}

