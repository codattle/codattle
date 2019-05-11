function initData() {
  return {
    score1: 0,
    score2: 0
  };
}

function parseChoice(choice) {
  return ["ROCK", "PAPER", "SCISSOR"].includes(choice) ? choice : "NO_CHOICE";
}

function isWin(firstChoice, secondChoice) {
  return (
    (firstChoice !== "NO_CHOICE" && secondChoice === "NO_CHOICE") ||
    (firstChoice === "ROCK" && secondChoice === "SCISSOR") ||
    (firstChoice === "PAPER" && secondChoice === "ROCK") ||
    (firstChoice === "SCISSOR" && secondChoice === "PAPER")
  );
}

var data = getCycle() === 0 ? initData() : loadData();

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

saveData(data);

if (data.score1 === 3) {
  end(0);
} else if (data.score2 === 3) {
  end(1);
} else if (getCycle() === 10) {
  end(null);
}

