type state = {
  selectedGameId: option(string),
  selectedMatchId: option(string),
  script: string,
};

type action =
  | SelectGame(option(string))
  | SelectMatch(option(string))
  | SetScript(string);

let component = ReasonReact.reducerComponent("App");

module SendScriptMutation = [%graphql
  {|
  mutation($gameId: ID!, $content: String!) {
    sendScript(gameId: $gameId, content: $content) {
      id
    }
  }
|}
];

module JoinMatchMutation = [%graphql
  {|
  mutation($matchId: ID!, $scriptId: ID!) {
    joinMatch(matchId: $matchId, scriptId: $scriptId)
  }
|}
];

let joinMatch = (script: string, gameId: string, matchId: string) => {
  GraphqlService.executeQuery(SendScriptMutation.make(~gameId, ~content=script, ()))
  |> Repromise.andThen(result =>
       result
       ->Belt.Option.flatMap(result => result##sendScript)
       ->Belt.Option.mapWithDefault(Repromise.resolved(None), script =>
           GraphqlService.executeQuery(JoinMatchMutation.make(~matchId, ~scriptId=script##id, ()))
         )
     )
  |> Repromise.wait(_ => ());
};

let make = _children => {
  ...component,
  initialState: () => {selectedGameId: None, selectedMatchId: None, script: ""},
  reducer: (action, state) =>
    switch (action) {
    | SelectGame(gameId) => ReasonReact.Update({...state, selectedGameId: gameId, selectedMatchId: None})
    | SelectMatch(matchId) => ReasonReact.Update({...state, selectedMatchId: matchId})
    | SetScript(script) => ReasonReact.Update({...state, script})
    },
  render: ({state, send}) => {
    <div>
      <h1> {ReasonReact.string("Welcome to Codattle")} </h1>
      <GameCreator />
      <GameSelector onSelect={gameId => send(SelectGame(Some(gameId)))} />
      {switch (state.selectedGameId) {
       | Some(gameId) =>
         <>
           <MatchSelector gameId onSelect={matchId => send(SelectMatch(Some(matchId)))} />
           {switch (state.selectedMatchId) {
            | Some(matchId) =>
              <>
                <ScriptEditor onChange={script => send(SetScript(script))} />
                <button onClick={_ => joinMatch(state.script, gameId, matchId)}> {ReasonReact.string("Join match")} </button>
              </>
            | None => <> </>
            }}
         </>
       | None => <> </>
       }}
    </div>;
  },
};