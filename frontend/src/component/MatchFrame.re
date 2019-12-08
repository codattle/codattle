open Rationale.Option;
open Rationale.RList;

type context = {fileIdBySpriteName: Belt.Map.String.t(string)};

module type Frame = {
  type content;

  let decodeOrRaise: (Js.Json.t, context) => content;
  let createElement: content => React.element;
};

module TextFrame: Frame = {
  type content = {output: string};

  let decodeOrRaise = (json, _context) => Json.Decode.{output: json |> field("output", string)};

  let createElement = frame => <span> {ReasonReact.string(frame.output)} </span>;
};

module CanvasFrame: Frame = {
  type gameObject = {
    x: int,
    y: int,
    width: int,
    height: int,
    spriteFileId: option(string),
  };

  type content = {
    width: int,
    height: int,
    objects: list(gameObject),
  };

  let decodeOrRaise = (json, context) =>
    Json.Decode.{
      width: json |> field("width", int),
      height: json |> field("height", int),
      objects:
        json
        |> field(
             "objects",
             list(gameObject =>
               {
                 x: gameObject |> field("x", int),
                 y: gameObject |> field("y", int),
                 width: gameObject |> field("width", int),
                 height: gameObject |> field("height", int),
                 spriteFileId: context.fileIdBySpriteName->Belt.Map.String.get(gameObject |> field("sprite", string)),
               }
             ),
           ),
    };

  let drawFrame = (frame, context) => {
    open Canvas;

    frame.objects
    |> List.map(({spriteFileId}) => spriteFileId)
    |> filter_opt
    |> List.map(fileId => Image.loadFromUrl(Environment.storageUrl ++ fileId) |> Repromise.map(image => (fileId, image)))
    |> Repromise.all
    |> Repromise.wait(images => {
         clear(context);
         frame.objects
         |> List.iter(({spriteFileId, x, y, width, height}) => {
              let spriteImage = spriteFileId |> flatMap(spriteFileId => images |> find(((fileId, _)) => fileId === spriteFileId));
              switch (spriteImage) {
              | Some((_, image)) => context |> drawImage(~image, ~x, ~y, ~width, ~height)
              | None => context |> drawFilledRectangle(~color=Color.black, ~x, ~y, ~width, ~height)
              };
            });
       });
    ();
  };

  // TODO: Performance issue: canvas is drawn twice
  let createElement = frame =>
    <CanvasComponent width={string_of_int(frame.width)} height={string_of_int(frame.height)} draw={drawFrame(frame)} />;
};

module MakeFrameElementGenerator = (Frame: Frame) => {
  let create = (frameJson, context) => {
    switch (Frame.decodeOrRaise(frameJson, context)) {
    | frame => Some(() => Frame.createElement(frame))
    | exception (Json.Decode.DecodeError(_)) => None
    };
  };
};

module TextFrameElementGenerator = MakeFrameElementGenerator(TextFrame);
module CanvasFrameElementGenerator = MakeFrameElementGenerator(CanvasFrame);

type state =
  | NotLoaded
  | Loaded(unit => React.element)
  | Failure;

module Styles = {
  open Css;

  let container = style([boxShadow(~blur=2 |> px, `hex("4A4A4A"))]);
};

[@react.component]
let make = (~frame, ~context) => {
  let (state, setState) = React.useState(() => NotLoaded);

  let frameGeneratorCreators =
    [|("text", TextFrameElementGenerator.create), ("2d", CanvasFrameElementGenerator.create)|]->Belt.Map.String.fromArray;

  let getFrameElementGenerator = frameJson => {
    Json.parse(frameJson)
    >>= (
      frame =>
        Json.Decode.(frame |> optional(field("mode", string)))
        >>= (mode => frameGeneratorCreators->Belt.Map.String.get(mode))
        >>= (frameGeneratorCreator => frameGeneratorCreator(frame, context))
    )
    <$> (frameGenerator => Loaded(frameGenerator))
    |> default(Failure);
  };

  React.useEffect1(
    () => {
      setState(_ => getFrameElementGenerator(frame));
      None;
    },
    [|frame|],
  );

  <div>
    {switch (state) {
     | NotLoaded => <> </>
     | Loaded(frameElementGenerator) => <div className=Styles.container> {frameElementGenerator()} </div>
     | Failure => <Translation id="matchFrame.errorWhileParsingFrame" />
     }}
  </div>;
};