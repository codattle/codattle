type mode =
  | Editing
  | Creating
  | Failure;

module CreateGameMutation = [%graphql
  {|
  mutation($name: String!, $mainLoop: String!) {
    createGame(name: $name, mainLoop: $mainLoop) {
      id
    }
  }
|}
];

[@react.component]
let make = () => {
  let (mode, setMode) = React.useState(() => Editing);
  let (name, setName) = React.useState(() => "");
  let (script, setScript) = React.useState(() => "");

  let createGame = () => {
    setMode(_ => Creating);
    GraphqlService.executeQuery(CreateGameMutation.make(~name, ~mainLoop=script, ()))
    |> Repromise.wait(result =>
         switch (result) {
         | Some(result) => ReasonReactRouter.push("/games/" ++ result##createGame##id)
         | None => setMode(_ => Failure)
         }
       );
  };

  switch (mode) {
  | Editing =>
    <div>
      <button onClick={_ => createGame()}> {ReasonReact.string("Create game")} </button>
      <div>
        <label htmlFor="gameName"> {ReasonReact.string("Name")} </label>
        <input id="gameName" value=name onChange={event => setName(ReactEvent.Form.target(event)##value)} />
      </div>
      <ScriptEditor onChange={script => setScript(_ => script)} />
    </div>
  | Creating => <div> {ReasonReact.string("Creating...")} </div>
  | Failure => <div> {ReasonReact.string("Creating fail :(")} </div>
  };
};