[@bs.config {jsx: 2}];

open Style.Color;

let rootStyle =
  Css.(style([hover([backgroundColor(primaryColor |> darken(0.15)) |> important]), backgroundColor(primaryColor) |> important]));
let labelStyle = Css.(style([color(textColor)]));

[@react.component]
let make = (~label, ~onChange=?) => {
  <MaterialUi.TextField
    label={ReasonReact.string(label)}
    onChange={event => onChange |> Utils.ifSome(onChange => onChange(ReactEvent.Form.target(event)##value))}
  />;
};