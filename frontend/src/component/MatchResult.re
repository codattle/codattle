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

module Styles = {
  open Css;

  let counter = style([display(`flex), fontSize(16 |> px), alignItems(`center)]);
  let actionMenu = style([display(`flex), marginTop(20 |> px), children([marginRight(20 |> px)])]);
  let section = style([marginTop(20 |> px), marginBottom(20 |> px)]);
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
    <div className=Styles.section> <WinnerComponent winner /> </div>
    {frames
     <$> (
       frames =>
         <>
           <MatchFrame frame={frames.selected.content} context />
           <div className=Styles.actionMenu>
             <div>
               <Button
                 label="common.previous"
                 disabled={!Selector.canPrevious(frames)}
                 onClick={() => onChange(Selector.Required.previous(frames))}
               />
             </div>
             <div>
               <Button
                 label="common.next"
                 disabled={!Selector.canNext(frames)}
                 onClick={() => onChange(Selector.Required.next(frames))}
               />
             </div>
             <span className=Styles.counter>
               {ReasonReact.string(string_of_int(Selector.Required.currentCount(frames)))}
               {ReasonReact.string(" / ")}
               {ReasonReact.string(string_of_int(Selector.Required.allCount(frames)))}
             </span>
           </div>
         </>
     )
     ||? <> </>}
  </div>;
};