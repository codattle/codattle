type frame = {output: option(string)};

type state =
  | NotLoaded
  | Loaded(frame)
  | Failure;

let decodeFrame = json => Json.parse(json)->Belt.Option.map(json => Json.Decode.{output: json |> optional(field("output", string))});

[@react.component]
let make = (~frame) => {
  let (state, setState) = React.useState(() => NotLoaded);

  React.useEffect1(
    () => {
      setState(_ => decodeFrame(frame)->Belt.Option.mapWithDefault(Failure, decodedFrame => Loaded(decodedFrame)));
      None;
    },
    [|frame|],
  );

  <div>
    {switch (state) {
     | NotLoaded => <> </>
     | Loaded(frame) => frame.output->Belt.Option.mapWithDefault(<> </>, output => <span> {ReasonReact.string(output)} </span>)
     | Failure => <Translation id="matchFrame.errorWhileParsingFrame" />
     }}
  </div>;
};