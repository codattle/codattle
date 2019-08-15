open Style.Color;

let rootStyle =
  Css.(style([hover([backgroundColor(primaryColor |> darken(0.15)) |> important]), backgroundColor(primaryColor) |> important]));
let labelStyle = Css.(style([color(textColor)]));

[@react.component]
let make = (~onClick=?, ~label=?, ~children=?, ~dataCy=?) =>
  ReasonReact.element(
    MaterialUi.Button.make(
      ~onClick=_ => onClick |> OptionUtils.ifSome(onClick => onClick()),
      ~variant=`Contained,
      ~classes=[Root(rootStyle), Label(labelStyle)],
      label->Belt.Option.mapWithDefault(children->Belt.Option.getWithDefault(<> </>), label => <Translation id=label />),
    ),
  )
  |> Utils.withDataCy(dataCy);