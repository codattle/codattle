let style = Css.(style([color(blue), marginLeft(5 |> px), marginRight(5 |> px), hover([color(lightblue)])]));

[@react.component]
let make = (~href, ~children) => {
  <a onClick={_ => ReasonReactRouter.push(href)} className=style> children </a>;
};