open OptionUtils;

[@react.component]
let make = (~label=?, ~onChange=?, ~rows=4, ~dataCy=?) => {
  ReasonReact.element(
    MaterialUi.TextField.make(
      ~label=label->Belt.Option.mapWithDefault(ReasonReact.string(""), label => <Translation id=label />),
      ~onChange=event => onChange |> execIfSome(ReactEvent.Form.target(event)##value),
      ~multiline=true,
      ~rows=`Int(rows),
      ~fullWidth=true,
      ~variant=`Outlined,
      [||],
    ),
  )
  |> Utils.withDataCy(dataCy);
};