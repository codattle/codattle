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
};

module GetGameQuery = [%graphql
  {|
  query($gameId: ID!) {
    game(gameId: $gameId) {
      id
      name
      logo {
        id
      }
      sprites {
        name
        image {
          id
        }
      }
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
    let logo = game.logo->Belt.Option.mapWithDefault(<> </>, logo => <img src={Environment.storageUrl ++ logo} width="64" height="64" />);
    let ratings =
      game.ratings
      |> Utils.componentList(rating => {
           let author = <span> {ReasonReact.string(rating.author)} </span>;
           let ratingValue = <Rating initialValue={rating.ratingValue} readOnly=true />;
           let description = rating.description <$> ReasonReact.string ||? <> </>;
           <div key={rating.author}> author ratingValue description </div>;
         });
    <div>
      logo
      <span> {ReasonReact.string("Details of game with id: " ++ game.id)} </span>
      <button onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/new-match")}> {ReasonReact.string("New match")} </button>
      <button onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/matches")}> {ReasonReact.string("See matches")} </button>
      <button onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/my-scripts")}>
        <Translation id="gameDetails.myScripts" />
      </button>
      ratings
      <SpriteList
        uploadedSprites={game.sprites}
        onUploadedSpriteRemove={spriteToRemoveName => removeSprite(game.id, spriteToRemoveName)}
        canAdd=false
      />
      <RatingForm onSend={({value, description}) => sendRating(value, description)} editRating=existsCurrentUserRating />
    </div>;
  });
};