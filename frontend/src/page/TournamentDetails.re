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

module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
  let title = style([fontSize(40 |> px), fontWeight(`bold), marginBottom(20 |> px)]);
};

[@react.component]
let make = (~tournamentId: string) => {
  let tournament =
    Utils.useResource(GetTournamentQuery.make(~tournamentId, ()), [|tournamentId|], data =>
      {name: data##tournament##name, canJoin: data##tournament##currentScriptCount < data##tournament##maxScriptCount}
    );

  tournament->Utils.displayResource(tournament => {
    let joinButton =
      tournament.canJoin
        ? <Button
            label="tournamentDetails.joinToTournament"
            onClick={_ => ReasonReactRouter.push("/games/tournaments/" ++ tournamentId ++ "/join")}
          />
        : <> </>;

    <div className=Styles.container> <div className=Styles.title> {ReasonReact.string(tournament.name)} </div> joinButton </div>;
  });
};