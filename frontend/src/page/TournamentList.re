type tournament = {
  id: string,
  name: string,
  scriptCount: int,
  maxScriptCount: int,
};

module GetTournamentsQuery = [%graphql
  {|
  query($gameId: ID!) {
    tournaments(gameId: $gameId) {
      id
      name
      scripts {
        id
      }
      maxScriptCount
    }
  }
|}
];

[@react.component]
let make = (~gameId) => {
  let (version, refresh) = Utils.useRefresh();

  let tournaments =
    Utils.useResource(GetTournamentsQuery.make(~gameId, ()), [|gameId, version|], data =>
      data##tournaments
      |> Array.to_list
      |> List.map(tournament =>
           {
             id: tournament##id,
             name: tournament##name,
             scriptCount: tournament##scripts |> Array.length,
             maxScriptCount: tournament##maxScriptCount,
           }
         )
    );

  tournaments->Utils.displayResource(tournaments => {
    let tournamentList =
      if (List.length(tournaments) == 0) {
        <Translation id="tournamentList.noTournaments" />;
      } else {
        <ul>
          {tournaments
           |> Utils.componentList(tournament =>
                <li key={tournament.id} onClick={_ => ReasonReactRouter.push("/games/tournaments/" ++ tournament.id)}>
                  {ReasonReact.string(
                     tournament.name
                     ++ " ("
                     ++ string_of_int(tournament.scriptCount)
                     ++ "/"
                     ++ string_of_int(tournament.maxScriptCount)
                     ++ ")",
                   )}
                </li>
              )}
        </ul>;
      };
    <div> <button onClick={_ => refresh()}> <Translation id="common.refresh" /> </button> tournamentList </div>;
  });
};