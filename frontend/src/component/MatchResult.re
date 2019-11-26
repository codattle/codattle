open Rationale.Option.Infix;
open OptionUtils.Infix;

type frame = {
  order: int,
  content: string,
};

let parseResultFrames = (frames: array(Js.t({..}))): option(Selector.Required.t(frame)) => {
  Array.sort((a, b) => a##order - b##order, frames);
  frames
  |> Array.map(frame => {order: frame##order, content: frame##content})
  |> Array.to_list
  |> Selector.Required.fromListWithFirstSelected;
};

[@react.component]
let make =
    (
      ~winner: option(int),
      ~frames: option(Selector.Required.t(frame)),
      ~context: MatchFrame.context,
      ~onChange: Selector.Required.t(frame) => unit,
    ) => {
  <div>
    <WinnerComponent winner />
    {frames
     <$> (
       frames =>
         <>
           <MatchFrame frame={frames.selected.content} context />
           <Button
             label="common.previous"
             disabled={!Selector.canPrevious(frames)}
             onClick={() => onChange(Selector.Required.previous(frames))}
           />
           <Button label="common.next" disabled={!Selector.canNext(frames)} onClick={() => onChange(Selector.Required.next(frames))} />
         </>
     )
     ||? <> </>}
  </div>;
};