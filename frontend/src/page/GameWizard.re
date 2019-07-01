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

module Styles = {
  open Css;

  let section = style([display(`flex), alignItems(`center), margin2(~v=20 |> px, ~h=10 |> px)]);
  let sectionElement = style([display(`inlineBlock), marginLeft(5 |> px), marginRight(5 |> px)]);
  let scriptEditor = merge([style([width(300 |> px), height(500 |> px)]), sectionElement]);
};

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
    Styles.(
      <div>
        <div className=section>
          <div className=sectionElement> <TextField label="Name" onChange={name => setName(_ => name)} /> </div>
          <div className=sectionElement> <Button onClick={_ => createGame()}> {ReasonReact.string("Create game")} </Button> </div>
        </div>
        <div className=section> <div className=scriptEditor> <ScriptEditor onChange={script => setScript(_ => script)} /> </div> </div>
      </div>
    )
  | Creating => <div> {ReasonReact.string("Creating...")} </div>
  | Failure => <div> {ReasonReact.string("Creating fail :(")} </div>
  };
};