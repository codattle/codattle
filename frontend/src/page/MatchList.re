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
      scripts {
        id
      }
      result {
        winner
      }
    }
  }
|}
];

[@react.component]
let make = (~gameId) => {
  let (version, refresh) = Utils.useRefresh();

  let matches =
    Utils.useResource(GetMatchesQuery.make(~gameId, ()), [|gameId, version|], data =>
      data##matches
      |> Array.to_list
      |> List.map(match =>
           {
             id: match##id,
             name: match##name,
             scriptsCount: Js.Array.length(match##scripts),
             winner: Belt.Option.mapWithDefault(match##result, None, result => result##winner),
           }
         )
    );

  let getMatchDescription = match =>
    match.name
    ++ " ("
    ++ string_of_int(match.scriptsCount)
    ++ "/2)"
    ++ Belt.Option.mapWithDefault(match.winner, "", winner => " Winner: Player " ++ string_of_int(winner + 1));

  <div>
    {switch (matches) {
     | NotLoaded => <> </>
     | Loading => <span> {ReasonReact.string("Loading...")} </span>
     | Loaded(matches) =>
       let matchList =
         if (List.length(matches) == 0) {
           <span> {ReasonReact.string("No matches")} </span>;
         } else {
           <ul>
             {matches
              |> Utils.componentList(match =>
                   <li key={match.id} onClick={_ => ReasonReactRouter.push("/games/matches/" ++ match.id)}>
                     {ReasonReact.string(getMatchDescription(match))}
                   </li>
                 )}
           </ul>;
         };
       <div> <button onClick={_ => refresh()}> {ReasonReact.string("Refresh")} </button> matchList </div>;
     | Failure => <span> {ReasonReact.string("Error")} </span>
     }}
  </div>;
};