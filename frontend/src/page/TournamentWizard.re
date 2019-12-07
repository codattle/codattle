open OptionUtils.Infix;

type mode =
  | Editing
  | Creating
  | Failure;

module CreateTournamentMutation = [%graphql
  {|
  mutation($name: String!, $gameId: ID!, $maxScriptCount: Int!) {
    createTournament(name: $name, gameId: $gameId, maxScriptCount: $maxScriptCount) {
      id
    }
  }
|}
];

let defaultPlayerCount = 4;

[@react.component]
let make = (~gameId) => {
  let (name, setName) = React.useState(() => "");
  let (maxScriptCount, setMaxScriptCount) = React.useState(() => defaultPlayerCount);
  let (mode, setMode) = React.useState(() => Editing);

  let createTournament = () => {
    setMode(_ => Creating);
    GraphqlService.executeQuery(CreateTournamentMutation.make(~name, ~gameId, ~maxScriptCount, ()))
    |> Repromise.wait(result =>
         switch (result) {
         | Belt.Result.Ok(result) => ReasonReactRouter.push("/games/tournaments/" ++ result##createTournament##id)
         | Error(_) => setMode(_ => Failure)
         }
       );
  };

  switch (mode) {
  | Editing =>
    <div>
      <input onChange={event => setName(ReactEvent.Form.target(event)##value)} />
      <NumberField
        label="tournamentWizard.playerCount"
        value=maxScriptCount
        onChange={maxScriptCount => setMaxScriptCount(_ => maxScriptCount ||? defaultPlayerCount)}
      />
      <button onClick={_ => createTournament()}> <Translation id="tournamentWizard.createTournament" /> </button>
    </div>
  | Creating => <Translation id="tournamentWizard.creating" />
  | Failure => <Translation id="tournamentWizard.errorWhileCreatingTournament" />
  };
};