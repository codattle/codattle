[@react.component]
let make = (~label=?, ~onChange=?, ~dataCy=?) => {
  Utils.withDataCy(
    ~dataCy,
    ReasonReact.element(
      MaterialUi.TextField.make(
        ~label=label->Belt.Option.mapWithDefault(ReasonReact.string(""), label => <Translation id=label />),
        ~onChange=event => onChange |> OptionUtils.ifSome(onChange => onChange(ReactEvent.Form.target(event)##value)),
        [||],
      ),
    ),
  );
};