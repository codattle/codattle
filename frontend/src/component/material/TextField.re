open OptionUtils;

[@react.component]
let make = (~label=?, ~onChange=?, ~dataCy=?) => {
  ReasonReact.element(
    MaterialUi.TextField.make(
      ~label=label->Belt.Option.mapWithDefault(ReasonReact.string(""), label => <Translation id=label />),
      ~onChange=event => onChange |> execIfSome(ReactEvent.Form.target(event)##value),
      [||],
    ),
  )
  |> Utils.withDataCy(dataCy);
};