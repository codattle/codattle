type game = {id: string};

type mode =
  | Editing
  | Joining
  | Failure;

module GetMatchQuery = [%graphql
  {|
  query($matchId: ID!) {
    match_: match(matchId: $matchId) {
      game {
        id
      }
    }
  }
|}
];

module SendScriptMutation = [%graphql
  {|
  mutation($gameId: ID!, $code: String!) {
    sendScript(gameId: $gameId, code: $code) {
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

[@react.component]
let make = (~matchId) => {
  let (script, setScript) = React.useState(() => "");
  let (mode, setMode) = React.useState(() => Editing);

  let game = Utils.useResource(GetMatchQuery.make(~matchId, ()), [|matchId|], data => {id: data##match##game##id});

  let joinMatchWithNewScript = (script: string, gameId: string, matchId: string) => {
    setMode(_ => Joining);
    GraphqlService.executeQuery(SendScriptMutation.make(~gameId, ~code=script, ()))
    |> Repromise.map(result => result->Belt.Option.map(result => result##sendScript))
    |> Repromise.andThen(createdScript =>
         createdScript->Belt.Option.mapWithDefault(Repromise.resolved(None), createdScript =>
           GraphqlService.executeQuery(JoinMatchMutation.make(~matchId, ~scriptId=createdScript##id, ()))
         )
       )
    |> Repromise.wait(result =>
         switch (result) {
         | Some(_) => ReasonReactRouter.push("/games/matches/" ++ matchId)
         | None => setMode(_ => Failure)
         }
       );
  };

  switch (game) {
  | NotLoaded => <div />
  | Loading => <div> {ReasonReact.string("Loading game...")} </div>
  | Loaded(game) =>
    switch (mode) {
    | Editing =>
      <div>
        <ScriptEditor onChange={script => setScript(_ => script)} />
        <button onClick={_ => joinMatchWithNewScript(script, game.id, matchId)}> {ReasonReact.string("Join match")} </button>
      </div>
    | Joining => <div> {ReasonReact.string("Joining match...")} </div>
    | Failure => <div> {ReasonReact.string("Error while joining game :(")} </div>
    }

  | Failure => <div> {ReasonReact.string("Error while loading game :(")} </div>
  };
};