type canvas;
type context;

external castDomToCanvas: Dom.element => canvas = "%identity";
[@bs.send.pipe: canvas] external getContext: string => context = "getContext";
[@bs.send.pipe: context] external drawImage: (Image.t, int, int, int, int) => unit = "drawImage";
[@bs.set] external setFillStyle: (context, string) => unit = "fillStyle";
[@bs.get] external getCanvas: (context) => canvas = "canvas";
[@bs.get] external getWidth: (canvas) => int = "clientWidth";
[@bs.get] external getHeight: (canvas) => int = "clientHeight";
[@bs.send.pipe: context] external fillRect: (int, int, int, int) => unit = "fillRect";
[@bs.send.pipe: context] external clearRect: (int, int, int, int) => unit = "clearRect";

let get2dContext = canvas => canvas |> getContext("2d");

let clear = context => {
  let canvas = getCanvas(context);
  context |> clearRect(0, 0, canvas |> getWidth, canvas |> getHeight)
};

let drawImage = (~image, ~x, ~y, ~width, ~height, context) => context |> drawImage(image, x, y, width, height);

let drawFilledRectangle = (~color, ~x, ~y, ~width, ~height, context) => {
  context->setFillStyle(color |> Color.hexToHexString);
  context |> fillRect(x, y, width, height);
};