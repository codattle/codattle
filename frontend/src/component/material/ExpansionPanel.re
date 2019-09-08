module Styles = {
  open Css;

  let withBorder = style([border(1 |> px, `solid, rgb(127, 127, 127)), borderRadius(4 |> px)]);
  let contentStyle = style([Css.important(cursor(`default)), Css.important(userSelect(`auto))]);
};

[@react.component]
let make = (~header, ~content) => {
  Styles.(
    ReasonReact.element(
      MaterialUi.ExpansionPanel.make(
        ~className=withBorder,
        [|
          ReasonReact.element(MaterialUi.ExpansionPanelSummary.make(ReasonReact.element(MaterialUi.Typography.make(header)))),
          ReasonReact.element(
            MaterialUi.ExpansionPanelDetails.make(~className=contentStyle, ReasonReact.element(MaterialUi.Typography.make(content))),
          ),
        |],
      ),
    )
  );
};