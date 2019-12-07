module Styles = {
  open Css;

  let root = style([backgroundColor(rgb(57, 66, 78)) |> important]);
};

[@react.component]
let make = (~position=?, ~children=?) =>
  ReasonReact.element(MaterialUi.AppBar.make(~position?, ~classes=[Root(Styles.root)], children));