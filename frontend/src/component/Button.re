open Style.Color;
open OptionUtils;
open Rationale.Option;

let rootStyle =
  Css.(style([hover([backgroundColor(primaryColor |> darken(0.15)) |> important]), backgroundColor(primaryColor) |> important]));
let labelStyle = Css.(style([color(textColor)]));

[@react.component]
let make = (~onClick=?, ~label=?, ~disabled=false, ~dataCy=?, ~children=?) =>
  ReasonReact.element(
    MaterialUi.Button.make(
      ~disabled,
      ~onClick=_ => onClick |> execIfSome(),
      ~variant=`Contained,
      ~classes=[Root(rootStyle), Label(labelStyle)],
      label |> fmap(label => <Translation id=label />) |> default(default(<> </>, children)),
    ),
  )
  |> Utils.withDataCy(dataCy);