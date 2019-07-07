type mode =
  | Editing
  | Creating
  | Failure;

module CreateGameMutation = [%graphql
  {|
  mutation($name: String!, $description: String!, $code: String!) {
    createGame(name: $name, description: $description code: $code) {
      id
    }
  }
|}
];

module Styles = {
  open Css;

  let section = style([margin2(~v=20 |> px, ~h=10 |> px)]);
  let scriptEditor = style([width(300 |> px), height(500 |> px)]);
};

[@react.component]
let make = () => {
  let (mode, setMode) = React.useState(() => Editing);
  let (name, setName) = React.useState(() => "");
  let (script, setScript) = React.useState(() => "");

  let createGame = () => {
    setMode(_ => Creating);
    GraphqlService.executeQuery(CreateGameMutation.make(~name, ~description="", ~code=script, ()))
    |> Repromise.wait(result =>
         switch (result) {
         | Some(result) => ReasonReactRouter.push("/games/" ++ result##createGame##id)
         | None => setMode(_ => Failure)
         }
       );
  };

  switch (mode) {
  | Editing =>
    Styles.(
      <div>
        <div className=section> <TextField label="gameWizard.name" onChange={name => setName(_ => name)} /> </div>
        <div className=section> <Button label="gameWizard.createGame" onClick={_ => createGame()} /> </div>
        <div className={j|$section $scriptEditor|j}> <ScriptEditor onChange={script => setScript(_ => script)} /> </div>
      </div>
    )
  | Creating => <div> <Translation id="gameWizard.creating" /> </div>
  | Failure => <div> <Translation id="gameWizard.errorWhileCreatingGame" /> </div>
  };
};