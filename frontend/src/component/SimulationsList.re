type simulation = {name: string};

type state =
  | Init
  | LoadingSimulations
  | LoadedSimulations(list(simulation))
  | Error;

type action =
  | FetchSimulations
  | SetSimulations(list(simulation))
  | SetError;

let component = ReasonReact.reducerComponent("SimulationsList");

module GetSimulationsQuery = [%graphql {|
    query {
        simulations {
            id
            name
        }
    }
|}];

let getSimulations = () => GraphqlService.executeQuery(GetSimulationsQuery.make());

let make = _children => {
  ...component,
  initialState: () => Init,
  reducer: (action, _state) =>
    switch (action) {
    | FetchSimulations =>
      ReasonReact.UpdateWithSideEffects(
        LoadingSimulations,
        ({send}) =>
          getSimulations()
          |> Repromise.wait(result =>
               switch (result) {
               | Some(data) =>
                 let simulationsList = data##simulations |> Array.to_list |> List.map(simulation => {name: simulation##name});
                 send(SetSimulations(simulationsList));
               | None => send(SetError)
               }
             ),
      )
    | SetSimulations(simulations) => ReasonReact.Update(LoadedSimulations(simulations))
    | SetError => ReasonReact.Update(Error)
    },
  didMount: ({send}) => {
    send(FetchSimulations);
  },
  render: ({state, send}) => {
    <div>
      {switch (state) {
       | Init => <> </>
       | LoadingSimulations => <span> {ReasonReact.string("Loading...")} </span>
       | LoadedSimulations(simulations) =>
         let simulationsList =
           if (List.length(simulations) == 0) {
             <span> {ReasonReact.string("No simulations")} </span>;
           } else {
             <ul>
               {simulations
                |> List.map(simulation => <li> {ReasonReact.string(simulation.name)} </li>)
                |> Array.of_list
                |> ReasonReact.array}
             </ul>;
           };
         <> <button onClick={_event => send(FetchSimulations)}> {ReasonReact.string("refresh")} </button> simulationsList </>;
       | Error => <span> {ReasonReact.string("Error")} </span>
       }}
    </div>;
  },
};