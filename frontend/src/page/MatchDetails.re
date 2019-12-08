open Rationale.Option.Infix;
open OptionUtils.Infix;

type sprite = {
  name: string,
  fileId: string,
};

type game = {sprites: list(sprite)};

type matchResult = {
  winner: option(int),
  frames: option(Selector.Required.t(MatchResult.frame)),
};

type match = {
  name: string,
  game,
  result: option(matchResult),
  full: bool,
};

module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
  let matchTitle = style([fontSize(40 |> px), fontWeight(`bold), marginBottom(20 |> px)]);
};

module GetMatchQuery = [%graphql
  {|
  query($matchId: ID!) {
    match_: match(matchId: $matchId) {
      name
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

[@react.component]
let make = (~matchId) => {
  let (match, setMatch) =
    Utils.useEditableResource(GetMatchQuery.make(~matchId, ()), [|matchId|], data =>
      {
        name: data##match##name,
        game: {
          sprites: data##match##game##sprites |> Js.Array.map(sprite => {name: sprite##name, fileId: sprite##image##id}) |> Array.to_list,
        },
        result:
          data##match##result
          ->Belt.Option.map(result => {winner: result##winner, frames: result##resultFrames |> MatchResult.parseResultFrames}),
        full: data##match##scripts |> Array.length === data##match##game##maxAllowedPlayerCount,
      }
    );

  match->Utils.displayResource(match => {
    let context =
      MatchFrame.{
        fileIdBySpriteName:
          match.game.sprites |> List.map((sprite: sprite) => (sprite.name, sprite.fileId)) |> Array.of_list |> Belt.Map.String.fromArray,
      };

    let result =
      match.result
      <$> (
        ({frames, winner}) =>
          <MatchResult
            frames
            winner
            context
            onChange={frames => setMatch(match => {...match, result: Some({frames: Some(frames), winner})})}
          />
      )
      ||? <> </>;

    let newScriptButton =
      match.full
        ? <> </>
        : <Button label="matchDetails.joinToMatch" onClick={_ => ReasonReactRouter.push("/games/matches/" ++ matchId ++ "/new-script")} />;

    <div className=Styles.container>
      <div className=Styles.matchTitle> {ReasonReact.string(match.name)} </div>
      result
      newScriptButton
    </div>;
  });
};