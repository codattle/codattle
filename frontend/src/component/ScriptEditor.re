[@react.component]
let make = (~onChange=?) => {
  <TextArea label="scriptEditor.code" onChange={script => onChange |> Utils.ifSome(onChange => onChange(script))} rows=20 />;
};