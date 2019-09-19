open OptionUtils.Infix;

type variantType = [ | `Default | `Negative];
type variant = {color: Css.color};

module Styles = {
  open Css;

  let icon = variant => style([SVG.fill(variant.color) |> important]);
  let select = variant =>
    style([
      color(variant.color) |> important,
      before([borderColor(variant.color) |> important]),
      after([borderColor(variant.color) |> important]),
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
let make =
    (
      ~value: Selector.Optional.t('a),
      ~nameMapper: 'a => React.element,
      ~onChange: Selector.Optional.t('a) => unit,
      ~variant: option(variantType)=?,
    ) => {
  let variant =
    switch (variant ||? `Default) {
    | `Default => {color: Css.black}
    | `Negative => {color: Css.white}
    };

  <MaterialSelect
    value={value.selected ||? Obj.magic("")}
    onChange={(event, _) =>
      onChange(value |> Selector.Optional.select(Rationale.Util.identical(ReactEvent.Form.target(event)##value)))
    }
    className={Styles.select(variant)}
    classes={MaterialSelect.classes(~icon=Styles.icon(variant), ())}>
    {value |> Selector.Optional.all |> Utils.componentList(value => <MenuItem value> {nameMapper(value)} </MenuItem>)}
  </MaterialSelect>;
};