open Rationale.Option.Infix;
open OptionUtils.Infix;

type rating = {
  author: string,
  ratingValue: int,
  description: option(string),
};

type game = {
  id: string,
  name: string,
  logo: option(string),
  sprites: list(SpriteList.uploadedSprite),
  ratings: list(rating),
  description: I18nText.t,
  code: string,
};

module GetGameQuery = [%graphql
  {|
  query($gameId: ID!) {
    game(gameId: $gameId) {
      id
      name
      description {
        language,
        content
      }
      logo {
        id
      }
      sprites {
        name
        image {
          id
        }
      }
      code
      ratings {
        author {
          username
        }
        value
        description
      }
    }
  }
|}
];

module RateGameMutation = [%graphql
  {|
  mutation($gameId: ID!, $rating: RatingValue!, $description: String) {
    rateGame(gameId: $gameId, rating: $rating, description: $description) {
      author {
        username
      }
      value
      description
    }
  }
|}
];

module RemoveSpriteFromGameMutation = [%graphql
  {|
  mutation($gameId: ID!, $spriteName: String!) {
    removeSpriteFromGame(gameId: $gameId, spriteName: $spriteName)
  }
|}
];

module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
  let section = style([display(`flex), alignItems(`center)]);
  let gameTitle = style([fontSize(40 |> px), fontWeight(`bold)]);
  let logo = style([marginRight(20 |> px)]);
  let actionMenu =
    style([
      marginTop(20 |> px),
      marginBottom(20 |> px),
      marginLeft((-10) |> px),
      children([margin2(~v=0 |> px, ~h=10 |> px) |> important]),
    ]);
  let ratings = style([marginTop(40 |> px), marginBottom(40 |> px)]);
  let code = style([height(400 |> px), marginTop(20 |> px), marginBottom(20 |> px)]);
  let panel = style([marginTop(20 |> px), marginBottom(20 |> px)]);
  let ratingAuthor = style([fontSize(16 |> px), fontWeight(`bold), marginBottom(5 |> px)]);
};

let mapRating = rating => {
  author: rating##author##username,
  ratingValue: rating##value |> Json.Decode.int,
  description: rating##description,
};

[@react.component]
let make = (~gameId) => {
  let (game, setGame) =
    Utils.useEditableResource(GetGameQuery.make(~gameId, ()), [|gameId|], data =>
      {
        id: data##game##id,
        name: data##game##name,
        logo: data##game##logo <$> (logo => logo##id),
        sprites: data##game##sprites |> ArrayUtils.mapToList(sprite => {name: sprite##name, SpriteList.fileId: sprite##image##id}),
        ratings: data##game##ratings |> ArrayUtils.mapToList(mapRating),
        description: data##game##description |> I18nText.fromJs,
        code: data##game##code,
      }
    );

  let removeSprite = (gameId: string, spriteName: string) => {
    GraphqlService.executeQuery(RemoveSpriteFromGameMutation.make(~gameId, ~spriteName, ()))
    |> Repromise.Rejectable.wait(response =>
         switch (response) {
         | Belt.Result.Ok(_) =>
           let filterSprites = sprites => sprites |> List.filter((x: SpriteList.uploadedSprite) => x.name !== spriteName);
           setGame(game => {...game, sprites: filterSprites(game.sprites)});
         | _ => ()
         }
       );
  };

  game->Utils.displayResource(game => {
    let sendRating = (value, description) => {
      GraphqlService.executeQuery(RateGameMutation.make(~gameId=game.id, ~rating=value |> Json.Encode.int, ~description?, ()))
      |> Repromise.wait(response =>
           switch (response) {
           | Belt.Result.Ok(response) =>
             let rating = response##rateGame |> mapRating;
             let isSameAuthor = (firstRating, secondRating) => firstRating.author === secondRating.author;
             setGame(game => {...game, ratings: game.ratings |> Rationale.RList.unionWith(isSameAuthor, [rating])});
           | _ => ()
           }
         );
    };
    let existsCurrentUserRating = game.ratings |> Rationale.RList.any(rating => rating.author === ProfileService.username);
    let logo =
      game.logo
      ->Belt.Option.mapWithDefault(<> </>, logo =>
          <img className=Styles.logo src={Environment.storageUrl ++ logo} width="64" height="64" />
        );
    let ratings =
      game.ratings
      |> Utils.componentList(rating => {
           let author = <div className=Styles.ratingAuthor> {ReasonReact.string(rating.author)} </div>;
           let ratingValue = <Rating initialValue={rating.ratingValue} readOnly=true />;
           let description = <div> {rating.description <$> ReasonReact.string ||? <> </>} </div>;
           <div key={rating.author}> author ratingValue description </div>;
         });
    <div className=Styles.container>
      <div className=Styles.section> logo <div className=Styles.gameTitle> {ReasonReact.string(game.name)} </div> </div>
      <div className=Styles.actionMenu>
        <Button label="gameDetails.newMatch" onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/new-match")} />
        <Button label="gameDetails.newTournament" onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/new-tournament")} />
        <Button label="gameDetails.tournaments" onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/tournaments")} />
        <Button label="gameDetails.matches" onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/matches")} />
        <Button label="gameDetails.myScripts" onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/my-scripts")} />
        <Button label="gameDetails.scriptTesting" onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/script-testing")} />
      </div>
      <div className=Styles.panel> <GameDescription description={game.description} /> </div>
      {if (List.length(game.sprites) > 0) {
         <div className=Styles.panel>
           <ExpansionPanel header="gameDetails.sprites">
             <SpriteList
               uploadedSprites={game.sprites}
               onUploadedSpriteRemove={spriteToRemoveName => removeSprite(game.id, spriteToRemoveName)}
               canAdd=false
             />
           </ExpansionPanel>
         </div>;
       } else {
         <> </>;
       }}
      <div className=Styles.code> <CodeEditor value={game.code} /> </div>
      <div className=Styles.ratings> ratings </div>
      <RatingForm onSend={({value, description}) => sendRating(value, description)} editRating=existsCurrentUserRating />
    </div>;
  });
};