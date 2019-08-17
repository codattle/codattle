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

  let sendLogo = () => {
    logo->Belt.Option.mapWithDefault(Repromise.Rejectable.resolved(None), logo =>
      FileRestEndpoint.sendFile(logo) |> Repromise.Rejectable.map(fileId => Some(fileId))
    );
  };

  let sendSprite = ({name, file}: SpriteList.notUploadedSprite) => {
    FileRestEndpoint.sendFile(file) |> Repromise.Rejectable.map(fileId => {"name": name, "image": fileId});
  };

  let sendSprites = () => {
    Repromise.Rejectable.all(sprites |> List.map(sendSprite));
  };

  let createGame = () => {
    setMode(_ => Creating);
    PromiseUtils.all2(sendLogo(), sendSprites())
    |> Repromise.Rejectable.andThen(((logo, sprites)) =>
         GraphqlService.executeQuery(
           CreateGameMutation.make(~name, ~description="", ~code=script, ~logo?, ~sprites=sprites |> Array.of_list, ()),
         )
         |> Repromise.Rejectable.catch(_error => Repromise.Rejectable.rejected(""))
       )
    |> Repromise.Rejectable.catch(_error => Repromise.resolved(None))
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
        <div className=section> <InputFile label="gameWizard.logo" onChange={file => setLogo(_ => Some(file))} dataCy="logo" /> </div>
        <div className=section> <TextField label="gameWizard.name" onChange={name => setName(_ => name)} dataCy="name" /> </div>
        <div className=section> <Button label="gameWizard.createGame" onClick={_ => createGame()} dataCy="create" /> </div>
        <div className={j|$section $scriptEditor|j}> <ScriptEditor onChange={script => setScript(_ => script)} dataCy="code" /> </div>
        <div className=section>
          <SpriteList notUploadedSprites=sprites onNotUploadedSpritesChange={sprites => setSprites(_ => sprites)} />
        </div>
      </div>
    )
  | Creating => <div> <Translation id="gameWizard.creating" /> </div>
  | Failure => <div> <Translation id="gameWizard.errorWhileCreatingGame" /> </div>
  };
};