[@bs.config {jsx: 2}];

open Style.Color;

let rootStyle =
  Css.(style([hover([backgroundColor(primaryColor |> darken(0.15)) |> important]), backgroundColor(primaryColor) |> important]));
let labelStyle = Css.(style([color(textColor)]));

[@react.component]
let make = (~onClick=?, ~children) => {
  <MaterialUi.Button
    onClick={_ => onClick |> Utils.ifSome(onClick => onClick())} variant=`Contained classes=[Root(rootStyle), Label(labelStyle)]>
    children
  </MaterialUi.Button>;
};