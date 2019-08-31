open Rationale.Option;
open Rationale.RList;

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

let mapRating = rating => {
  author: rating##author##username,
  ratingValue: rating##value |> Json.Decode.int,
  description: rating##description,
};

// TODO: remove after implementing authorization with Keycloak
let username = "test";

[@react.component]
let make = (~gameId) => {
  let (game, setGame) =
    Utils.useEditableResource(GetGameQuery.make(~gameId, ()), [|gameId|], data =>
      {
        id: data##game##id,
        name: data##game##name,
        logo: data##game##logo |> fmap(logo => logo##id),
        sprites: data##game##sprites |> Array.map(sprite => {name: sprite##name, SpriteList.fileId: sprite##image##id}) |> Array.to_list,
        ratings: data##game##ratings |> Array.map(mapRating) |> Array.to_list,
      }
    );

  switch (game) {
  | NotLoaded => <div />
  | Loading => <span> {ReasonReact.string("Loading...")} </span>
  | Loaded(game) =>
    let sendRating = (value, description) => {
      GraphqlService.executeQuery(RateGameMutation.make(~gameId=game.id, ~rating=value |> Json.Encode.int, ~description?, ()))
      |> Repromise.Rejectable.wait(response =>
           switch (response) {
           | Some(response) =>
             let rating = response##rateGame |> mapRating;
             let isSameAuthor = (firstRating, secondRating) => firstRating.author === secondRating.author;
             setGame(game => {...game, ratings: game.ratings |> unionWith(isSameAuthor, [rating])});
           | None => ()
           }
         );
    };
    let existsCurrentUserRating = game.ratings |> any(rating => rating.author === username);
    let logo = game.logo->Belt.Option.mapWithDefault(<> </>, logo => <img src={Environment.storageUrl ++ logo} width="64" height="64" />);
    let ratings =
      game.ratings
      |> Utils.componentList(rating => {
           let author = <span> {ReasonReact.string(rating.author)} </span>;
           let ratingValue = <Rating initialValue={rating.ratingValue} readOnly=true />;
           let description =
             rating.description |> fmap(description => <span> {ReasonReact.string(description)} </span>) |> default(<> </>);
           <div> author ratingValue description </div>;
         });
    <div>
      logo
      <span> {ReasonReact.string("Details of game with id: " ++ game.id)} </span>
      <button onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/new-match")}> {ReasonReact.string("New match")} </button>
      <button onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/matches")}> {ReasonReact.string("See matches")} </button>
      ratings
      <SpriteList uploadedSprites={game.sprites} canAdd=false />
      <RatingForm onSend={({value, description}) => sendRating(value, description)} editRating=existsCurrentUserRating />
    </div>;
  | Failure => <Translation id="common.error" />
  };
};