open Rationale.Option;

type context = {fileIdBySpriteName: Belt.Map.String.t(string)};

module type Frame = {
  type content;

  let decodeOrRaise: Js.Json.t => content;
  let createElement: (content, context) => React.element;
};

module TextFrame: Frame = {
  type content = {output: string};

  let decodeOrRaise = json => Json.Decode.{output: json |> field("output", string)};

  let createElement = (frame, _context) => <span> {ReasonReact.string(frame.output)} </span>;
};

module CanvasFrame: Frame = {
  type gameObject = {
    x: int,
    y: int,
    width: int,
    height: int,
    spriteName: string,
  };

  type content = {
    width: int,
    height: int,
    objects: list(gameObject),
  };

  let decodeOrRaise = json =>
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
                 spriteName: gameObject |> field("sprite", string),
               }
             ),
           ),
    };

  let drawFrame = (~frame, ~fileIdBySpriteName, canvas) => {
    open Canvas;

    canvas
    |> Js.Nullable.toOption
    |> iter(canvas => {
         let context = canvas |> refToCanvas |> get2dContext;
         frame.objects
         |> List.iter(({spriteName, x, y, width, height}) =>
              switch (fileIdBySpriteName->Belt.Map.String.get(spriteName)) {
              | Some(fileId) => context |> drawImageFromUrl(~url=Environment.storageUrl ++ fileId, ~x, ~y, ~width, ~height)
              | None => context |> drawFilledRectangle(~color=Color.black, ~x, ~y, ~width, ~height)
              }
            );
       });
    ();
  };

  let createElement = (frame, context) =>
    <canvas
      width={string_of_int(frame.width)}
      height={string_of_int(frame.height)}
      ref={ReactDOMRe.Ref.callbackDomRef(drawFrame(~frame, ~fileIdBySpriteName=context.fileIdBySpriteName))}
    />;
};

module MakeFrameElementGenerator = (Frame: Frame) => {
  let create = frameJson => {
    switch (Frame.decodeOrRaise(frameJson)) {
    | frame => Some(context => Frame.createElement(frame, context))
    | exception (Json.Decode.DecodeError(_)) => None
    };
  };
};

module TextFrameElementGenerator = MakeFrameElementGenerator(TextFrame);
module CanvasFrameElementGenerator = MakeFrameElementGenerator(CanvasFrame);

type state =
  | NotLoaded
  | Loaded(context => React.element)
  | Failure;

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
        >>= (frameGeneratorCreator => frameGeneratorCreator(frame))
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
     | Loaded(frameElementGenerator) => frameElementGenerator(context)
     | Failure => <Translation id="matchFrame.errorWhileParsingFrame" />
     }}
  </div>;
};