module Styles = {
  open Css;

  let header = style([fontWeight(`bold)]);
  let withBorder = style([border(1 |> px, `solid, rgb(127, 127, 127)), borderRadius(4 |> px)]);
  let contentStyle = style([Css.important(cursor(`default)), Css.important(userSelect(`auto))]);
};

[@react.component]
let make = (~header, ~children) => {
  ReasonReact.element(
    MaterialUi.ExpansionPanel.make(
      ~className=Styles.withBorder,
      [|
        ReasonReact.element(
          MaterialUi.ExpansionPanelSummary.make(
            ReasonReact.element(MaterialUi.Typography.make(<Translation id=header className=Styles.header />)),
          ),
        ),
        ReasonReact.element(
          MaterialUi.ExpansionPanelDetails.make(
            ~className=Styles.contentStyle,
            ReasonReact.element(MaterialUi.Typography.make(children)),
          ),
        ),
      |],
    ),
  );
};