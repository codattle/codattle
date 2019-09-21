open OptionUtils.Infix;
open Rationale.Option.Infix;

[@react.component]
let make = (~label: option(string)=?, ~value: option(int)=?, ~onChange: option(option(int) => unit)=?, ~dataCy: option(string)=?) => {
  let (displayValue, setDisplayValue) = React.useState(() => value <$> string_of_int ||? "");

  ReasonReact.element(
    MaterialUi.TextField.make(
      ~type_="number",
      ~label=label <$> (label => <Translation id=label />) ||? ReasonReact.string(""),
      ~value=`String(displayValue),
      ~onChange=
        event => {
          let value = ReactEvent.Form.target(event)##value;
          setDisplayValue(_ => value);
          value |> Utils.intOfString |?> onChange;
        },
      ~onBlur=_ => setDisplayValue(_ => value <$> string_of_int ||? ""),
      ~inputProps={"min": 0, "step": 1},
      [||],
    ),
  )
  |> Utils.withDataCy(dataCy);
};