open OptionUtils.Infix;
open Rationale.Option.Infix;

module Styles = {
  open Css;
  open Style.Color;

  let containedStyle =
    style([hover([backgroundColor(primaryColor |> darken(0.15)) |> important]), backgroundColor(primaryColor) |> important]);
  let labelStyle = style([color(textColor)]);
};

[@react.component]
let make =
    (
      ~onClick=?,
      ~label=?,
      ~variant=`Contained,
      ~disabled=false,
      ~dataCy=?,
      ~disableRipple: bool=?,
      ~component: option(string)=?,
      ~children=?,
    ) =>
  ReasonReact.element(
    MaterialUi.Button.make(
      ~disabled,
      ~onClick=_ => () |?> onClick,
      ~variant,
      ~classes=[Label(Styles.labelStyle), Contained(Styles.containedStyle)],
      ~disableRipple,
      ~component=?component->Belt.Option.map(component => `String(component)),
      label <$> (label => <Translation id=label />) |? children ||? <> </>,
    ),
  )
  |> Utils.withDataCy(dataCy);