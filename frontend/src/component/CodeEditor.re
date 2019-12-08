open OptionUtils;

module AceEditor = {
  [@bs.module "react-ace"] [@react.component]
  external make:
    (
      ~mode: string,
      ~theme: string,
      ~className: string=?,
      ~width: string=?,
      ~height: string=?,
      ~value: string=?,
      ~onChange: (string, option(Js.t({..}))) => unit=?
    ) =>
    React.element =
    "default";
};

module Styles = {
  open Css;

  let editorContainer =
    style([width(100.0 |> pct), height(100.0 |> pct), minHeight(200 |> px), position(`relative), boxShadow(~blur=2 |> px, `hex("4A4A4A"))]);
  let editor = style([position(`absolute), top(0 |> px), right(0 |> px), bottom(0 |> px), left(0 |> px)]);
};

[@react.component]
let make = (~value, ~onChange=?, ~dataCy=?) => {
  <div className=Styles.editorContainer>
    <AceEditor
      value
      onChange={(value, _event) => onChange |> execIfSome(value)}
      mode="javascript"
      theme="tomorrow"
      className=Styles.editor
      width="auto"
      height="100%"
    />
  </div>
  |> Utils.withDataCy(dataCy);
};