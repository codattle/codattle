open Rationale.Function.Infix;
open Rationale.Option.Infix;
open OptionUtils.Infix;

type mode =
  | Editing
  | Creating
  | Failure;

module CreateGameMutation = [%graphql {|
  mutation($game: NewGame) {
    createGame(game: $game) {
      id
    }
  }
|}];

module Styles = {
  open Css;

  let container = style([display(`flex), padding(30 |> px), marginTop((-20) |> px)]);
  let left = style([width(50.0 |> pct)]);
  let right = style([flexGrow(1.0), paddingLeft(30 |> px)]);
  let section = style([margin2(~v=20 |> px, ~h=0 |> px)]);
  let input = style([width(100.0 |> pct), margin2(~v=0 |> px, ~h=10 |> px) |> important]);
  let sectionGroup = style([display(`flex), margin2(~v=20 |> px, ~h=(-10) |> px)]);
  let scriptEditor = style([width(100.0 |> pct), height(500 |> px)]);
};

let defaultPlayerCount = 2;

[@react.component]
let make = () => {
  let (mode, setMode) = React.useState(() => Editing);
  let (name, setName) = React.useState(() => "");
  let (description, setDescription) = React.useState(() => "");
  let (script, setScript) = React.useState(() => "");
  let (playerCount, setPlayerCount) = React.useState(() => defaultPlayerCount);
  let (logo: option(File.t), setLogo) = React.useState(() => None);
  let (sprites, setSprites) = React.useState(() => []);

  let sendLogo = (): PromiseUtils.t(option(string)) =>
    logo <$> (FileRestEndpoint.sendFile ||> Repromise.mapOk(x => Some(x))) ||? PromiseUtils.resolved(None);

  let sendSprite = ({name, file}: SpriteList.notUploadedSprite): PromiseUtils.t(Js.t({..})) =>
    FileRestEndpoint.sendFile(file) |> Repromise.mapOk(fileId => {"name": name, "image": fileId});

  let sendSprites = (): PromiseUtils.t(list(Js.t({..}))) => sprites |> List.map(sendSprite) |> PromiseUtils.all;

  let createGame = () => {
    setMode(_ => Creating);
    PromiseUtils.all2(sendLogo(), sendSprites())
    |> Repromise.andThenOk(((logo, sprites)) =>
         GraphqlService.executeQuery(
           CreateGameMutation.make(
             ~game={
               "name": name,
               "description": description,
               "code": script,
               "logo": logo,
               "sprites": Some(sprites |> Array.of_list),
               "allowedPlayerCounts": [|playerCount|],
             },
             (),
           ),
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
      <div className=Styles.container>
        <div className=Styles.left>
          <div className=section>
            <InputFileButton label="gameWizard.addLogo" file=logo onChange={file => setLogo(_ => Some(file))} />
          </div>
          <div className=Styles.sectionGroup>
            <TextField
              label="gameWizard.name"
              onChange={name => setName(_ => name)}
              variant=`Outlined
              className=Styles.input
              dataCy="name"
            />
            <NumberField
              label="gameWizard.playerCount"
              value=playerCount
              onChange={playerCount => setPlayerCount(_ => playerCount ||? defaultPlayerCount)}
              variant=`Outlined
              className=Styles.input
            />
          </div>
          <div className=section>
            <TextArea
              label="gameWizard.description"
              rows=5
              onChange={description => setDescription(_ => description)}
              dataCy="description"
            />
          </div>
          <div className={j|$section $scriptEditor|j}>
            <CodeEditor value=script onChange={script => setScript(_ => script)} dataCy="code" />
          </div>
          <div className=section> <Button label="gameWizard.createGame" onClick={_ => createGame()} dataCy="create" /> </div>
        </div>
        <div className=Styles.right>
          <div className=section>
            <SpriteList notUploadedSprites=sprites onNotUploadedSpritesChange={sprites => setSprites(_ => sprites)} />
          </div>
        </div>
      </div>
    )
  | Creating => <div> <Translation id="gameWizard.creating" /> </div>
  | Failure => <div> <Translation id="gameWizard.errorWhileCreatingGame" /> </div>
  };
};