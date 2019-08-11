[@react.component]
let make = (~onChange=?, ~dataCy=?) =>
  <TextArea label="scriptEditor.code" onChange={script => onChange |> OptionUtils.ifSome(onChange => onChange(script))} rows=20 dataCy />;