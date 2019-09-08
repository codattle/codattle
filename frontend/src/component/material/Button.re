open Style.Color;
open OptionUtils;
open Rationale.Option;

let containedStyle =
  Css.(style([hover([backgroundColor(primaryColor |> darken(0.15)) |> important]), backgroundColor(primaryColor) |> important]));
let labelStyle = Css.(style([color(textColor)]));

[@react.component]
let make = (~onClick=?, ~label=?, ~variant=`Contained, ~disabled=false, ~dataCy=?, ~children=?) =>
  ReasonReact.element(
    MaterialUi.Button.make(
      ~disabled,
      ~onClick=_ => onClick |> execIfSome(),
      ~variant,
      ~classes=[Label(labelStyle), Contained(containedStyle)],
      label |> fmap(label => <Translation id=label />) |> default(default(<> </>, children)),
    ),
  )
  |> Utils.withDataCy(dataCy);