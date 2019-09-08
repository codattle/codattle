module Styles = {
  open Style.Color;
  open Css;

  let icon = style([SVG.fill(textColor) |> important]);
  let select =
    style([
      color(textColor) |> important,
      before([borderColor(textColor) |> important]),
      after([borderColor(textColor) |> important]),
    ]);
};

module MenuItem = {
  [@bs.module "@material-ui/core/MenuItem"] [@react.component]
  external make: (~value: 'value=?, ~children: 'children=?) => React.element = "default";
};

module MaterialSelect = {
  [@bs.deriving abstract]
  type classes = {
    [@bs.optional]
    icon: string,
  };

  [@bs.module "@material-ui/core/Select"] [@react.component]
  external make:
    (
      ~value: 'value,
      ~onChange: (ReactEvent.Form.t, 'child) => unit=?,
      ~className: string=?,
      ~classes: classes=?,
      ~children: 'children=?
    ) =>
    React.element =
    "default";
};

[@react.component]
let make = (~value: 'value, ~items: list(('value, React.element)), ~onChange: option('value => unit)=?) =>
  <MaterialSelect
    value
    onChange={(event, _) => onChange |> OptionUtils.execIfSome(ReactEvent.Form.target(event)##value)}
    className=Styles.select
    classes={MaterialSelect.classes(~icon=Styles.icon, ())}>
    {items |> Utils.componentList(((value, content)) => <MenuItem value> content </MenuItem>)}
  </MaterialSelect>;