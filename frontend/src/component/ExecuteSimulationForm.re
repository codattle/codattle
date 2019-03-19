type state =
  | Editing(string)
  | Sending;

type action =
  | SetName(string)
  | Send;

let component = ReasonReact.reducerComponent("ExecuteSimulationForm");

module ExecuteSimulationQuery = [%graphql
  {|
    mutation($name: String!) {
        executeSimulation(name: $name) {
          id
        }
    }
|}
];

let executeSimulation = name => GraphqlService.executeQuery(ExecuteSimulationQuery.make(~name, ()));

let make = _children => {
  ...component,
  initialState: () => Editing(""),
  reducer: (action, state) =>
    switch (action) {
    | SetName(name) => ReasonReact.Update(Editing(name))
    | Send =>
      switch (state) {
      | Editing(name) =>
        ReasonReact.UpdateWithSideEffects(
          Sending,
          ({send}) => executeSimulation(name) |> Repromise.wait(_result => send(SetName(""))),
        )
      | _ => ReasonReact.NoUpdate
      }
    },
  render: ({state, send}) => {
    switch (state) {
    | Editing(name) =>
      <>
        <input value=name onChange={event => send(SetName(ReactEvent.Form.target(event)##value))} />
        <button onClick={_event => send(Send)}> {ReasonReact.string("Send")} </button>
      </>
    | Sending => <span> {ReasonReact.string("Sending...")} </span>
    };
  },
};