type matchResult = {
  winner: option(int),
  frames: ArrayWithSelectedItem.t(string),
};

type match = {
  id: string,
  result: option(matchResult),
};

module GetMatchQuery = [%graphql
  {|
  query($matchId: ID!) {
    match_: match(matchId: $matchId) {
      game {
        id
      }
      result {
        winner
        resultFrames {
          order
          content
        }
      }
    }
  }
|}
];

let parseResultFrames = resultFrames => {
  Array.sort((a, b) => a##order - b##order, resultFrames);
  resultFrames |> Array.map(resultFrame => resultFrame##content) |> ArrayWithSelectedItem.fromArray;
};

[@react.component]
let make = (~matchId) => {
  let (match, setMatch) =
    Utils.useEditableResource(GetMatchQuery.make(~matchId, ()), [|matchId|], data =>
      {
        id: data##match##game##id,
        result:
          data##match##result->Belt.Option.map(result => {winner: result##winner, frames: result##resultFrames |> parseResultFrames}),
      }
    );

  switch (match) {
  | NotLoaded => <div />
  | Loading => <div> {ReasonReact.string("Loading...")} </div>
  | Loaded(match) =>
    let result =
      match.result
      ->Belt.Option.mapWithDefault(
          <> </>,
          result => {
            let winner =
              result.winner
              ->Belt.Option.mapWithDefault(<> </>, winner => <div> {ReasonReact.string("Winner: " ++ string_of_int(winner + 1))} </div>);
            let frame =
              result.frames->ArrayWithSelectedItem.getSelected->Belt.Option.mapWithDefault(<> </>, frame => <MatchFrame frame />);
            let nextFrameButton =
              ArrayWithSelectedItem.canNext(result.frames)
                ? <Button
                    label="common.next"
                    onClick={() =>
                      setMatch(match => {...match, result: Some({...result, frames: ArrayWithSelectedItem.next(result.frames)})})
                    }
                  />
                : <> </>;
            let previousFrameButton =
              ArrayWithSelectedItem.canPrevious(result.frames)
                ? <Button
                    label="common.previous"
                    onClick={() =>
                      setMatch(match => {...match, result: Some({...result, frames: ArrayWithSelectedItem.previous(result.frames)})})
                    }
                  />
                : <> </>;
            <div> winner frame nextFrameButton previousFrameButton </div>;
          },
        );

    <div>
      <div> {ReasonReact.string("Details of match with id: " ++ matchId)} </div>
      result
      <button onClick={_ => ReasonReactRouter.push("/games/matches/" ++ matchId ++ "/new-script")}>
        {ReasonReact.string("New script")}
      </button>
    </div>;
  | Failure => <div> {ReasonReact.string("Failure while loading match :(")} </div>
  };
};