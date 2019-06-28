[@react.component]
let make = (~onChange=?) => {
  let (script, setScript) = React.useState(() => "");

  let changeScript = script => {
    setScript(_ => script);
    Belt.Option.mapWithDefault(onChange, (), onChange => onChange(script));
  };

  <textarea value=script onChange={event => changeScript(ReactEvent.Form.target(event)##value)} />;
};