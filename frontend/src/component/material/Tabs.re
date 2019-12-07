type value;

let selected: string => value = [%raw string => "{return string}"];
let unselected: value = [%raw "false"];

[@bs.module "@material-ui/core/Tabs"] [@react.component]
external make:
  (~value: value=?, ~onChange: ('event, string) => unit=?, ~className: string=?, ~classes: Js.Dict.t(string)=?, ~children: 'children=?) =>
  React.element =
  "default";