type game = {
  id: string,
  name: string,
};

module GetGamesQuery = [%graphql {|
  query {
    games {
      id
      name
    }
  }
|}];

[@react.component]
let make = () => {
  let (version, refresh) = Utils.useRefresh();

  let games =
    Utils.useResource(GetGamesQuery.make(), [|version|], data =>
      data##games |> Array.to_list |> List.map(game => {id: game##id, name: game##name})
    );

  <div>
    {switch (games) {
     | NotLoaded => <div />
     | Loading => <span> {ReasonReact.string("Loading...")} </span>
     | Loaded(games) =>
       let gameList =
         if (List.length(games) == 0) {
           <span> {ReasonReact.string("No games")} </span>;
         } else {
           <ul>
             {games
              |> Utils.componentList(game =>
                   <li onClick={_ => ReasonReactRouter.push("/games/" ++ game.id)} key={game.id}> {ReasonReact.string(game.name)} </li>
                 )}
           </ul>;
         };
       <div> <button onClick={_ => refresh()}> {ReasonReact.string("Refresh")} </button> gameList </div>;
     | Failure => <span> {ReasonReact.string("Error")} </span>
     }}
  </div>;
};