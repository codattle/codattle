[@react.component]
let make = (~variant=?, ~children=?) => ReasonReact.element(MaterialUi.Typography.make(~variant?, children));