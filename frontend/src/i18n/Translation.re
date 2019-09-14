[@react.component]
let make = (~id, ~className=?) => {
  <span ?className> {ReasonReact.element(ReactIntl.FormattedMessage.make(~id, ~defaultMessage="", [||]))} </span>;
};