open Rationale.Option;

type uploadedSprite = {
  name: string,
  fileId: string,
};

type notUploadedSprite = {
  name: string,
  file: File.t,
};

module Styles = {
  open Css;

  let section = style([margin2(~v=20 |> px, ~h=(-16) |> px)]);
  let spriteElement = style([display(`flex), alignItems(`center), children([margin2(~v=0 |> px, ~h=20 |> px)])]);
};

module AddSpriteDialog = {
  module Styles = {
    open Css;

    let container = style([padding(20 |> px), children([margin2(~v=20 |> px, ~h=0 |> px), width(100.0 |> pct)])]);
  };

  type form = {
    name: string,
    file: option(File.t),
  };

  [@react.component]
  let make = (~_open, ~otherSpriteNames: list(string), ~onClose) => {
    let (form, setForm) = React.useState(() => {name: "", file: None});

    let isNameUnique = name => !(otherSpriteNames |> List.exists(otherSpriteName => otherSpriteName == name));
    let isNameValid = name => String.length(name) > 0 && isNameUnique(name);
    let getSprite = () =>
      switch (form.file) {
      | Some(file) => isNameValid(form.name) ? Some({name: form.name, file}: notUploadedSprite) : None
      | None => None
      };
    let isFormInvalid = () => getSprite() |> Belt.Option.isNone;
    let setName = name => setForm(form => {...form, name});
    let setFile = file => setForm(form => {...form, file: Some(file)});

    <div>
      <Dialog _open onClose={() => onClose(None)}>
        <div className=Styles.container>
          <TextField label="spriteList.addSpriteDialog.name" onChange=setName variant=`Outlined />
          <InputFile onChange=setFile />
          <Button label="common.add" disabled={isFormInvalid()} onClick={() => onClose(getSprite())} />
        </div>
      </Dialog>
    </div>;
  };
};

[@react.component]
let make =
    (
      ~uploadedSprites: list(uploadedSprite)=[],
      ~notUploadedSprites: list(notUploadedSprite)=[],
      ~onNotUploadedSpritesChange=?,
      ~onNotUploadedSpriteAdded=?,
      ~onUploadedSpriteRemove=?,
      ~canAdd=true,
    ) => {
  let (dialogOpen, setDialogOpen) = React.useState(() => false);

  let addSprite = sprite => {
    onNotUploadedSpriteAdded |> OptionUtils.execIfSome(sprite);
    onNotUploadedSpritesChange |> OptionUtils.execIfSome(notUploadedSprites @ [sprite]);
  };

  let handleDialogClose = sprite => {
    sprite |> iter(addSprite);
    setDialogOpen(_ => false);
  };

  let isAnySprite = List.length(uploadedSprites) > 0 || List.length(notUploadedSprites) > 0;

  let spriteNames = {
    (uploadedSprites |> List.map((sprite: uploadedSprite) => sprite.name))
    @ (notUploadedSprites |> List.map((sprite: notUploadedSprite) => sprite.name));
  };

  let removeNotUploaded = (allSprites: list(notUploadedSprite), spriteToRemoveName: string) => {
    let otherSprites = allSprites |> List.filter((sprite: notUploadedSprite) => sprite.name !== spriteToRemoveName);
    onNotUploadedSpritesChange |> OptionUtils.execIfSome(otherSprites);
  };

  let removeUploaded = (spriteToRemoveName: string) => {
    onUploadedSpriteRemove |> OptionUtils.execIfSome(spriteToRemoveName);
  };

  let spriteElements =
    <div>
      {
        let width = "50";
        let height = "50";
        (
          uploadedSprites
          |> List.map(({name, fileId}) =>
               <div key=name className=Styles.spriteElement>
                 <span> {ReasonReact.string(name)} </span>
                 <img src={Environment.storageUrl ++ fileId} width height />
                 <Button label="common.remove" onClick={() => removeUploaded(name)} />
               </div>
             )
        )
        @ (
          notUploadedSprites
          |> List.map(({name, file}) =>
               <div key=name className=Styles.spriteElement>
                 <div> {ReasonReact.string(name)} </div>
                 <ImagePreview image=file width height />
                 <Button label="common.remove" onClick={() => removeNotUploaded(notUploadedSprites, name)} />
               </div>
             )
        )
        |> Array.of_list
        |> React.array;
      }
    </div>;
  <div>
    {canAdd ? <Button label="spriteList.addSprite" onClick={() => setDialogOpen(_ => true)} /> : <> </>}
    {isAnySprite ? <div className=Styles.section> spriteElements </div> : <> </>}
    <AddSpriteDialog _open=dialogOpen otherSpriteNames=spriteNames onClose=handleDialogClose />
  </div>;
};