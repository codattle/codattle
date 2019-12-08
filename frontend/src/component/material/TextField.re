open OptionUtils;

[@react.component]
let make =
    (
      ~label: option(string)=?,
      ~onChange: option(string => unit)=?,
      ~variant=?,
      ~className: option(string)=?,
      ~dataCy: option(string)=?,
    ) => {
  ReasonReact.element(
    MaterialUi.TextField.make(
      ~label=label->Belt.Option.mapWithDefault(ReasonReact.string(""), label => <Translation id=label />),
      ~onChange=event => onChange |> execIfSome(ReactEvent.Form.target(event)##value),
      ~variant?,
      ~className?,
      [||],
    ),
  )
  |> Utils.withDataCy(dataCy);
};