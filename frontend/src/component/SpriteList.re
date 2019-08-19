open Rationale.Option;

type uploadedSprite = {
  name: string,
  fileId: string,
};

type notUploadedSprite = {
  name: string,
  file: File.t,
};

module AddSpriteDialog = {
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
        <TextField onChange=setName />
        <InputFile onChange=setFile />
        <Button label="common.add" disabled={isFormInvalid()} onClick={() => onClose(getSprite())} />
      </Dialog>
    </div>;
  };
};

[@react.component]
let make =
    (
      ~uploadedSprites: list(uploadedSprite)=[],
      ~notUploadedSprites=[],
      ~onNotUploadedSpritesChange=?,
      ~onNotUploadedSpriteAdded=?,
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
  let spriteNames =
    (uploadedSprites |> List.map((sprite: uploadedSprite) => sprite.name)) @ (notUploadedSprites |> List.map(sprite => sprite.name));

  let spriteElements =
    <div>
      {
        let width = "50";
        let height = "50";
        (
          uploadedSprites
          |> List.map(({name, fileId}) =>
               <div key=name> <span> {ReasonReact.string(name)} </span> <img src={Environment.storageUrl ++ fileId} width height /> </div>
             )
        )
        @ (
          notUploadedSprites
          |> List.map(({name, file}) =>
               <div key=name> <span> {ReasonReact.string(name)} </span> <ImagePreview image=file width height /> </div>
             )
        )
        |> Array.of_list
        |> React.array;
      }
    </div>;

  <div>
    {canAdd ? <Button label="spriteList.addSprite" onClick={() => setDialogOpen(_ => true)} /> : <> </>}
    spriteElements
    <AddSpriteDialog _open=dialogOpen otherSpriteNames=spriteNames onClose=handleDialogClose />
  </div>;
};