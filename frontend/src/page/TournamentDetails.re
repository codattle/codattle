type tournament = {
  name: string,
  canJoin: bool,
};

module GetTournamentQuery = [%graphql
  {|
  query($tournamentId: ID!) {
    tournament(tournamentId: $tournamentId) {
      name
      currentScriptCount
      maxScriptCount
    }
  }
|}
];

[@react.component]
let make = (~tournamentId: string) => {
  let tournament =
    Utils.useResource(GetTournamentQuery.make(~tournamentId, ()), [|tournamentId|], data =>
      {name: data##tournament##name, canJoin: data##tournament##currentScriptCount < data##tournament##maxScriptCount}
    );

  tournament->Utils.displayResource(tournament => {
    let joinButton =
      tournament.canJoin
        ? <button onClick={_ => ReasonReactRouter.push("/games/tournaments/" ++ tournamentId ++ "/join")}>
            <Translation id="tournamentDetails.joinToTournament" />
          </button>
        : <> </>;

    <div> <div> <Translation id="tournamentDetails.detailsOfTournament" /> </div> joinButton </div>;
  });
};