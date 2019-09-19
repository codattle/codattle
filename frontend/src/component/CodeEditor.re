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

  let editorContainer = style([width(300 |> px), height(400 |> px), position(`relative)]);
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