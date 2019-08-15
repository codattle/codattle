[@react.component]
let make = (~label=?, ~onChange=?, ~dataCy=?) => {
  ReasonReact.element(
    MaterialUi.TextField.make(
      ~label=label->Belt.Option.mapWithDefault(ReasonReact.string(""), label => <Translation id=label />),
      ~onChange=event => onChange |> OptionUtils.ifSome(onChange => onChange(ReactEvent.Form.target(event)##value)),
      [||],
    ),
  )
  |> Utils.withDataCy(dataCy);
};