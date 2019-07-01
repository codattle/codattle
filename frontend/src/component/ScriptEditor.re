module Styles = {
  open Css;

  let fillParent = style([width(`percent(100.0)), height(`percent(100.0))]);
};

[@react.component]
let make = (~onChange=?) => {
  let (script, setScript) = React.useState(() => "");

  let changeScript = script => {
    setScript(_ => script);
    Belt.Option.mapWithDefault(onChange, (), onChange => onChange(script));
  };

  <textarea className=Styles.fillParent value=script onChange={event => changeScript(ReactEvent.Form.target(event)##value)} />;
};