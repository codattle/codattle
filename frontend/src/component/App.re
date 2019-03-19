let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <div> <h1> {ReasonReact.string("Welcome to Codattle")} </h1> <ExecuteSimulationForm /> <SimulationsList /> </div>,
};