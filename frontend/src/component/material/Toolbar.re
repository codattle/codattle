module Styles = {
  open Css;

  let root = style([minHeight(0 |> px) |> important]);
};

[@react.component]
let make = (~children=?) => ReasonReact.element(MaterialUi.Toolbar.make(~classes=[Root(Styles.root)], children));