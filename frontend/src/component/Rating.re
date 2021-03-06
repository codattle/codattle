[@bs.deriving abstract]
type params = {
  emptySymbol: React.element,
  fullSymbol: React.element,
  onChange: option(int => unit),
  initialRating: int,
  readonly: bool,
  fractions: int,
};

[@bs.module "react-rating"] external ratingElement: React.component(params) = "default";

module Styles = {
  open Css;

  let container = style([color(gold)]);
};

[@react.component]
let make = (~onChange: option(int => unit)=?, ~initialValue: int=3, ~readOnly: bool=false, ~fractions: int=1) => {
  <span className=Styles.container>
    {React.createElement(
       ratingElement,
       params(
         ~emptySymbol={<FontAwesomeIcon icon=Icon.Regular.star />},
         ~fullSymbol={<FontAwesomeIcon icon=Icon.Solid.star />},
         ~onChange,
         ~initialRating=initialValue,
         ~readonly=readOnly,
         ~fractions,
       ),
     )}
  </span>;
};