[@react.component]
let make = (~width, ~height, ~draw=?) => {
  let canvasRef = React.useRef(Js.Nullable.null);

  React.useEffect(() => {
    let canvasDom = canvasRef |> React.Ref.current |> Js.Nullable.toOption;
    (draw, canvasDom) |> OptionUtils.ifSome2((draw, canvasDom) => canvasDom |> Canvas.castDomToCanvas |> Canvas.get2dContext |> draw);
    None;
  });

  <canvas width height ref={ReactDOMRe.Ref.domRef(canvasRef)} />;
};