context("Games", () => {
  it("Create game", () => {
    cy.visit("/new-game");

    cy.get("[data-cy=name] input").type("Rock-Paper-Scissors");
    cy.fixture("rock-paper-scissors.txt").then(code => {
      cy.get("[data-cy=code] textarea").type(code);
    });
    cy.get("[data-cy=create]").click();

    cy.contains(/Details of game with id: \d+/);
  });
});
