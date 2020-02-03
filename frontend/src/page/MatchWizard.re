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

module Styles = {
  open Css;

  let container = style([padding(30 |> px)]);
  let createButton = style([marginTop(20 |> px)]);
};

[@react.component]
let make = (~gameId) => {
  let (name, setName) = React.useState(() => "");
  let (mode, setMode) = React.useState(() => Editing);

  let createMatch = () => {
    setMode(_ => Creating);
    GraphqlService.executeQuery(CreateMatchMutation.make(~name, ~gameId, ()))
    ->Promise.get(result =>
         switch (result) {
         | Belt.Result.Ok(result) => ReasonReactRouter.push("/games/matches/" ++ result##createMatch##id)
         | Error(_) => setMode(_ => Failure)
         }
       );
  };

  switch (mode) {
  | Editing =>
    <div className=Styles.container>
      <TextField label="matchWizard.name" onChange={name => setName(_ => name)} variant=`Outlined />
      <div className=Styles.createButton> <Button label="matchWizard.createMatch" onClick={_ => createMatch()} /> </div>
    </div>
  | Creating => ReasonReact.string("Creating...")
  | Failure => ReasonReact.string("Creating fail :(")
  };
};