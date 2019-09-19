type sprite = {
  name: string,
  fileId: string,
};

type game = {sprites: list(sprite)};

type matchResult = {
  winner: option(int),
  frames: ArrayWithSelectedItem.t(string),
};

type match = {
  game,
  result: option(matchResult),
  full: bool,
};

module GetMatchQuery = [%graphql
  {|
  query($matchId: ID!) {
    match_: match(matchId: $matchId) {
      game {
        sprites {
          name
          image {
            id
          }
        }
        maxAllowedPlayerCount
      }
      result {
        winner
        resultFrames {
          order
          content
        }
      }
      scripts {
        id
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
        game: {
          sprites: data##match##game##sprites |> Js.Array.map(sprite => {name: sprite##name, fileId: sprite##image##id}) |> Array.to_list,
        },
        result:
          data##match##result->Belt.Option.map(result => {winner: result##winner, frames: result##resultFrames |> parseResultFrames}),
        full: data##match##scripts |> Array.length === data##match##game##maxAllowedPlayerCount,
      }
    );

  match->Utils.displayResource(match => {
    let result =
      match.result
      ->Belt.Option.mapWithDefault(
          <> </>,
          result => {
            let winner =
              result.winner
              ->Belt.Option.mapWithDefault(<> </>, winner => <div> {ReasonReact.string("Winner: " ++ string_of_int(winner + 1))} </div>);
            let context =
              MatchFrame.{
                fileIdBySpriteName:
                  match.game.sprites |> List.map(sprite => (sprite.name, sprite.fileId)) |> Array.of_list |> Belt.Map.String.fromArray,
              };
            let frame =
              result.frames->ArrayWithSelectedItem.getSelected->Belt.Option.mapWithDefault(<> </>, frame => <MatchFrame frame context />);
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

    let newScriptButton =
      match.full
        ? <> </>
        : <button onClick={_ => ReasonReactRouter.push("/games/matches/" ++ matchId ++ "/new-script")}>
            {ReasonReact.string("New script")}
          </button>;

    <div> <div> {ReasonReact.string("Details of match with id: " ++ matchId)} </div> result newScriptButton </div>;
  });
};