module Styles = {
  open Css;

  [@bs.module] external programmingImage: {. "default": string} = "../static/images/programming.png";

  let container = style([backgroundColor(hex("564F8E")), minHeight(`calc((`sub, 100.0 |> vh, 48 |> px))), display(`flex)]);
  let left = style([width(40.0 |> pct), display(`flex), alignItems(`center), paddingLeft(40 |> px)]);
  let image = style([display(`block), width(90.0 |> pct), margin2(~v=0 |> px, ~h=`auto)]);
  let right = style([display(`flex), alignItems(`center), flexGrow(1.0), paddingRight(40 |> px)]);
  let headers =
    style([
      display(`flex),
      flexDirection(`column),
      alignItems(`center),
      marginLeft(`auto),
      marginRight(`auto),
      fontSize(60 |> px),
      fontWeight(`bolder),
      color(white),
      textShadows([textShadow(~blur=2 |> px, black)]),
      flexGrow(1.0),
      textAlign(`center),
    ]);
  let titleHeader = style([fontSize(80 |> px)]);
};

[@react.component]
let make = () =>
  <div>
    <div className=Styles.container>
      <div className=Styles.left> <img src={Styles.programmingImage##default} className=Styles.image /> </div>
      <div className=Styles.right>
        <div className=Styles.headers>
          <div className=Styles.titleHeader> {ReasonReact.string("Codattle")} </div>
          <div> {ReasonReact.string("Learn coding by playing games")} </div>
        </div>
      </div>
    </div>
  </div>;