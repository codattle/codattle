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

module Styles = {
  open Css;

  let container = style([padding(30 |> px)]);
  let section = style([marginBottom(20 |> px)]);
};

[@react.component]
let make = (~gameId) => {
  let (name, setName) = React.useState(() => "");
  let (maxScriptCount, setMaxScriptCount) = React.useState(() => defaultPlayerCount);
  let (mode, setMode) = React.useState(() => Editing);

  let createTournament = () => {
    setMode(_ => Creating);
    GraphqlService.executeQuery(CreateTournamentMutation.make(~name, ~gameId, ~maxScriptCount, ()))
    ->Promise.get(result =>
         switch (result) {
         | Belt.Result.Ok(result) => ReasonReactRouter.push("/games/tournaments/" ++ result##createTournament##id)
         | Error(_) => setMode(_ => Failure)
         }
       );
  };

  switch (mode) {
  | Editing =>
    <div className=Styles.container>
      <div className=Styles.section>
        <TextField label="tournamentWizard.name" variant=`Outlined onChange={name => setName(_ => name)} />
      </div>
      <div className=Styles.section>
        <NumberField
          variant=`Outlined
          label="tournamentWizard.playerCount"
          value=maxScriptCount
          onChange={maxScriptCount => setMaxScriptCount(_ => maxScriptCount ||? defaultPlayerCount)}
        />
      </div>
      <div className=Styles.section>
        <Button onClick={_ => createTournament()}> <Translation id="tournamentWizard.createTournament" /> </Button>
      </div>
    </div>
  | Creating => <Translation id="tournamentWizard.creating" />
  | Failure => <Translation id="tournamentWizard.errorWhileCreatingTournament" />
  };
};