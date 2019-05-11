type state = {
  name: string,
  script: string,
};

type action =
  | SetName(string)
  | SetScript(string)
  | CreateGame;

let component = ReasonReact.reducerComponent("GameCreator");

module CreateGameMutation = [%graphql
  {|
  mutation($name: String!, $mainLoop: String!) {
    createGame(name: $name, mainLoop: $mainLoop) {
      id
    }
  }
|}
];

let createGame = (name: string, script: string) =>
  GraphqlService.executeQuery(CreateGameMutation.make(~name, ~mainLoop=script, ())) |> Repromise.wait(_ => ());

let make = _children => {
  ...component,
  initialState: () => {name: "", script: ""},
  reducer: (action, state) =>
    switch (action) {
    | SetName(name) => ReasonReact.Update({...state, name})
    | SetScript(script) => ReasonReact.Update({...state, script})
    | CreateGame => ReasonReact.SideEffects(_self => createGame(state.name, state.script))
    },
  render: ({state, send}) => {
    <div>
      <button onClick={_event => send(CreateGame)}> {ReasonReact.string("Create game")} </button>
      <div>
        <label htmlFor="gameName"> {ReasonReact.string("Name")} </label>
        <input id="gameName" value={state.name} onChange={event => send(SetName(ReactEvent.Form.target(event)##value))} />
      </div>
      <ScriptEditor onChange={script => send(SetScript(script))} />
    </div>;
  },
};