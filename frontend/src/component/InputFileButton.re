module Styles = {
  open Css;

  let container = style([display(`flex), alignItems(`center)]);
  let input = style([display(`none)]);
  let label = style([marginRight(30 |> px)]);
};

[@react.component]
let make = (~label: string, ~file: option(File.t), ~onChange=?) => {
  let getFileFromEvent = (event): option(File.t) => {
    let event = ReactEvent.Form.target(event);
    event##validity##valid ? ListUtils.first(event##files) : None;
  };

  <div className=Styles.container>
    <input
      className=Styles.input
      id="contained-button-file"
      type_="file"
      onChange={event => (onChange, getFileFromEvent(event)) |> OptionUtils.ifSome2((onChange, file) => onChange(file))}
    />
    <label className=Styles.label htmlFor="contained-button-file"> <Button label component="span" /> </label>
    {switch (file) {
     | Some(file) => <ImagePreview image=file width="60" height="60" />
     | None => <> </>
     }}
  </div>;
};