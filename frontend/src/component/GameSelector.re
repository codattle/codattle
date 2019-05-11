type game = {
  id: string,
  name: string,
};

type state =
  | NotLoaded
  | Loading
  | Loaded(list(game))
  | Error;

type action =
  | LoadGames
  | SetGames(list(game))
  | SetError;

let component = ReasonReact.reducerComponent("GameSelector");

module GetGamesQuery = [%graphql {|
  query {
    games {
      id
      name
    }
  }
|}];

let getGames = () => GraphqlService.executeQuery(GetGamesQuery.make());

let make = (~onSelect=?, _children) => {
  ...component,
  initialState: () => NotLoaded,
  reducer: (action, _state) =>
    switch (action) {
    | LoadGames =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        ({send}) =>
          getGames()
          |> Repromise.wait(result =>
               switch (result) {
               | Some(data) =>
                 let games = data##games |> Array.to_list |> List.map(game => {id: game##id, name: game##name});
                 send(SetGames(games));
               | None => send(SetError)
               }
             ),
      )
    | SetGames(games) => ReasonReact.Update(Loaded(games))
    | SetError => ReasonReact.Update(Error)
    },
  didMount: ({send}) => {
    send(LoadGames);
  },
  render: ({state, send}) => {
    let onSelect = gameId => {
      switch (onSelect) {
      | Some(onSelect) => onSelect(gameId)
      | None => ()
      };
    };

    <div>
      {switch (state) {
       | NotLoaded => <> </>
       | Loading => <span> {ReasonReact.string("Loading...")} </span>
       | Loaded(games) =>
         let gameList =
           if (List.length(games) == 0) {
             <span> {ReasonReact.string("No games")} </span>;
           } else {
             <ul>
               {games
                |> List.map(game => <li onClick={_event => onSelect(game.id)} key={game.id}> {ReasonReact.string(game.name)} </li>)
                |> Array.of_list
                |> ReasonReact.array}
             </ul>;
           };
         <> <button onClick={_event => send(LoadGames)}> {ReasonReact.string("Refresh")} </button> gameList </>;
       | Error => <span> {ReasonReact.string("Error")} </span>
       }}
    </div>;
  },
};