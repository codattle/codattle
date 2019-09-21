[@react.component]
let make = (~position=?, ~children=?) => ReasonReact.element(MaterialUi.AppBar.make(~position?, children));