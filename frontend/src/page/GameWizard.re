open Rationale.Option;
open Rationale.Function.Infix;

type mode =
  | Editing
  | Creating
  | Failure;

module CreateGameMutation = [%graphql
  {|
  mutation($name: String!, $description: String!, $code: String!, $logo: ID, $sprites: [NewSprite!]) {
    createGame(name: $name, description: $description, code: $code, logo: $logo, sprites: $sprites) {
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
  let (logo: option(File.t), setLogo) = React.useState(() => None);
  let (sprites, setSprites) = React.useState(() => []);

  let sendLogo = (): PromiseUtils.t(option(string)) =>
    logo |> map(FileRestEndpoint.sendFile ||> Repromise.mapOk(some)) |> default(PromiseUtils.resolved(None));

  let sendSprite = ({name, file}: SpriteList.notUploadedSprite): PromiseUtils.t(Js.t({..})) =>
    FileRestEndpoint.sendFile(file) |> Repromise.mapOk(fileId => {"name": name, "image": fileId});

  let sendSprites = (): PromiseUtils.t(list(Js.t({..}))) => sprites |> List.map(sendSprite) |> PromiseUtils.all;

  let createGame = () => {
    setMode(_ => Creating);
    PromiseUtils.all2(sendLogo(), sendSprites())
    |> Repromise.andThenOk(((logo, sprites)) =>
         GraphqlService.executeQuery(
           CreateGameMutation.make(~name, ~description="", ~code=script, ~logo?, ~sprites=sprites |> Array.of_list, ()),
         )
       )
    |> Repromise.wait(result =>
         switch (result) {
         | Belt.Result.Ok(result) => ReasonReactRouter.push("/games/" ++ result##createGame##id)
         | Error(_) => setMode(_ => Failure)
         }
       );
  };

  switch (mode) {
  | Editing =>
    Styles.(
      <div>
        <div className=section> <InputFile label="gameWizard.logo" onChange={file => setLogo(_ => Some(file))} dataCy="logo" /> </div>
        <div className=section> <TextField label="gameWizard.name" onChange={name => setName(_ => name)} dataCy="name" /> </div>
        <div className=section> <Button label="gameWizard.createGame" onClick={_ => createGame()} dataCy="create" /> </div>
        <div className={j|$section $scriptEditor|j}>
          <ScriptEditor value=script onChange={script => setScript(_ => script)} dataCy="code" />
        </div>
        <div className=section>
          <SpriteList notUploadedSprites=sprites onNotUploadedSpritesChange={sprites => setSprites(_ => sprites)} />
        </div>
      </div>
    )
  | Creating => <div> <Translation id="gameWizard.creating" /> </div>
  | Failure => <div> <Translation id="gameWizard.errorWhileCreatingGame" /> </div>
  };
};