[@react.component]
let make = (~label=?, ~onChange=?, ~rows=4) => {
  ReasonReact.element(
    MaterialUi.TextField.make(
      ~label=label->Belt.Option.mapWithDefault(ReasonReact.string(""), label => <Translation id=label />),
      ~onChange=event => onChange |> Utils.ifSome(onChange => onChange(ReactEvent.Form.target(event)##value)),
      ~multiline=true,
      ~rows=`Int(rows),
      ~fullWidth=true,
      ~variant=`Outlined,
      [||],
    ),
  );
};