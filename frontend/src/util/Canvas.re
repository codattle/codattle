type canvas;
type context;

external refToCanvas: Dom.element => canvas = "%identity";
[@bs.send.pipe: canvas] external getContext: string => context = "getContext";
[@bs.send.pipe: context] external drawImage: (Image.t, int, int, int, int) => unit = "drawImage";
[@bs.set] external setFillStyle: (context, string) => unit = "fillStyle";
[@bs.send.pipe: context] external fillRect: (int, int, int, int) => unit = "fillRect";

let get2dContext = canvas => canvas |> getContext("2d");

let drawImageFromUrl = (~url, ~x, ~y, ~width, ~height, context) => {
  Image.loadFromUrl(url, image => context |> drawImage(image, x, y, width, height));
};

let drawFilledRectangle = (~color, ~x, ~y, ~width, ~height, context) => {
  context->setFillStyle(color |> Color.hexToHexString);
  context |> fillRect(x, y, width, height);
};