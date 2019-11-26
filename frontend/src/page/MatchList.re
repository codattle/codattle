type match = {
  id: string,
  name: string,
  scriptsCount: int,
  winner: option(int),
};

module GetMatchesQuery = [%graphql
  {|
  query($gameId: ID!) {
    matches(gameId: $gameId) {
      id
      name
      scriptsCount
      result {
        winner
      }
    }
  }
|}
];

module GetMaxCountOfScripts = [%graphql {|
  query($gameId: ID!) {
    game(gameId: $gameId) {
      maxAllowedPlayerCount
    }
  }
|}];

[@react.component]
let make = (~gameId) => {
  let (version, refresh) = Utils.useRefresh();

  let maxCountOfScripts =
    Utils.useResource(GetMaxCountOfScripts.make(~gameId, ()), [|gameId, version|], data => data##game##maxAllowedPlayerCount);

  let matches =
    Utils.useResource(GetMatchesQuery.make(~gameId, ()), [|gameId, version|], data =>
      data##matches
      |> Array.to_list
      |> List.map(match =>
           {
             id: match##id,
             name: match##name,
             scriptsCount: match##scriptsCount,
             winner: Belt.Option.mapWithDefault(match##result, None, result => result##winner),
           }
         )
    );

  let getMatchDescription = (~maxCountOfScripts, ~match) => {
    <div>
      {ReasonReact.string(match.name ++ " (" ++ string_of_int(match.scriptsCount) ++ "/" ++ string_of_int(maxCountOfScripts) ++ ")")}
      <WinnerComponent winner={match.winner} />
    </div>;
  };

  maxCountOfScripts->Utils.displayResource(maxCountOfScripts =>
    matches->Utils.displayResource(matches => {
      let matchList =
        if (List.length(matches) == 0) {
          <span> {ReasonReact.string("No matches")} </span>;
        } else {
          <ul>
            {matches
             |> Utils.componentList(match =>
                  <li key={match.id} onClick={_ => ReasonReactRouter.push("/games/matches/" ++ match.id)}>
                    {getMatchDescription(~maxCountOfScripts, ~match)}
                  </li>
                )}
          </ul>;
        };
      <div> <button onClick={_ => refresh()}> <Translation id="common.refresh" /> </button> matchList </div>;
    })
  );
};