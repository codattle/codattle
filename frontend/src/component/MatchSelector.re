type match = {
  id: string,
  name: string,
  scriptsCount: int,
  winner: option(int),
};

type state =
  | NotLoaded
  | Loading
  | Loaded(string, list(match))
  | Error;

type action =
  | LoadMatches(string)
  | SetNewMatchName(string)
  | SetMatches(list(match))
  | SetError;

type retainedProps = {gameId: string};

let component = ReasonReact.reducerComponentWithRetainedProps("MatchSelector");

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

module CreateMatchMutation = [%graphql
  {|
  mutation($name: String!, $gameId: ID!) {
    createMatch(name: $name, gameId: $gameId) {
      id
    }
  }
|}
];

let getMatches = gameId => GraphqlService.executeQuery(GetMatchesQuery.make(~gameId, ()));

let make = (~gameId, ~onSelect=?, _children) => {
  ...component,
  initialState: () => NotLoaded,
  reducer: (action, state) =>
    switch (action) {
    | LoadMatches(gameId) =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        ({send}) =>
          getMatches(gameId)
          |> Repromise.wait(result =>
               switch (result) {
               | Some(data) =>
                 let matches =
                   data##matches
                   |> Array.to_list
                   |> List.map(match =>
                        {
                          id: match##id,
                          name: match##name,
                          scriptsCount: Js.Array.length(match##scripts),
                          winner: Belt.Option.mapWithDefault(match##result, None, result => result##winner),
                        }
                      );
                 send(SetMatches(matches));
               | None => send(SetError)
               }
             ),
      )
    | SetMatches(matches) => ReasonReact.Update(Loaded("", matches))
    | SetNewMatchName(newMatchName) =>
      switch (state) {
      | Loaded(_, matches) => ReasonReact.Update(Loaded(newMatchName, matches))
      | _ => ReasonReact.NoUpdate
      }
    | SetError => ReasonReact.Update(Error)
    },
  didMount: ({send}) => {
    send(LoadMatches(gameId));
  },
  retainedProps: {
    gameId: gameId,
  },
  willReceiveProps: ({retainedProps, send, state}) => {
    if (retainedProps.gameId !== gameId) {
      send(LoadMatches(gameId));
    };
    state;
  },
  render: ({state, send}) => {
    let createMatch = (name, gameId) =>
      GraphqlService.executeQuery(CreateMatchMutation.make(~name, ~gameId, ()))
      |> Repromise.wait(result =>
           if (Belt.Option.isSome(result)) {
             send(LoadMatches(gameId));
           }
         );

    let getMatchDescription = match =>
      match.name
      ++ " ("
      ++ string_of_int(match.scriptsCount)
      ++ "/2)"
      ++ Belt.Option.mapWithDefault(match.winner, "", winner => " Winner: Player " ++ string_of_int(winner + 1));

    <div>
      {switch (state) {
       | NotLoaded => <> </>
       | Loading => <span> {ReasonReact.string("Loading...")} </span>
       | Loaded(newMatchName, matches) =>
         let onSelect = matchId =>
           switch (onSelect) {
           | Some(onSelect) => onSelect(matchId)
           | None => ()
           };

         let matchList =
           if (List.length(matches) == 0) {
             <span> {ReasonReact.string("No matches")} </span>;
           } else {
             <ul>
               {matches
                |> List.map(match =>
                     <li key={match.id} onClick={_event => onSelect(match.id)}> {ReasonReact.string(getMatchDescription(match))} </li>
                   )
                |> Array.of_list
                |> ReasonReact.array}
             </ul>;
           };
         <>
           <button onClick={_event => send(LoadMatches(gameId))}> {ReasonReact.string("Refresh")} </button>
           <input value=newMatchName onChange={event => send(SetNewMatchName(ReactEvent.Form.target(event)##value))} />
           <button onClick={_event => createMatch(newMatchName, gameId)}> {ReasonReact.string("Create")} </button>
           matchList
         </>;
       | Error => <span> {ReasonReact.string("Error")} </span>
       }}
    </div>;
  },
};