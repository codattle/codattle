type mode =
  | Editing
  | Creating
  | Failure;

module CreateMatchMutation = [%graphql
  {|
  mutation($name: String!, $gameId: ID!) {
    createMatch(name: $name, gameId: $gameId) {
      id
    }
  }
|}
];

[@react.component]
let make = (~gameId) => {
  let (name, setName) = React.useState(() => "");
  let (mode, setMode) = React.useState(() => Editing);

  let createMatch = () => {
    setMode(_ => Creating);
    GraphqlService.executeQuery(CreateMatchMutation.make(~name, ~gameId, ()))
    |> Repromise.wait(result =>
         switch (result) {
         | Belt.Result.Ok(result) => ReasonReactRouter.push("/games/matches/" ++ result##createMatch##id)
         | Error(_) => setMode(_ => Failure)
         }
       );
  };

  switch (mode) {
  | Editing =>
    <div>
      <input onChange={event => setName(ReactEvent.Form.target(event)##value)} />
      <button onClick={_ => createMatch()}> {ReasonReact.string("Create match")} </button>
    </div>
  | Creating => ReasonReact.string("Creating...")
  | Failure => ReasonReact.string("Creating fail :(")
  };
};