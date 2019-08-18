type game = {
  id: string,
  name: string,
  logo: option(string),
  sprites: list(SpriteList.uploadedSprite),
};

module GetGameQuery = [%graphql
  {|
  query($gameId: ID!) {
    game(gameId: $gameId) {
      id
      name
      logo
      sprites {
        name
        image
      }
    }
  }
|}
];

[@react.component]
let make = (~gameId) => {
  let game =
    Utils.useResource(GetGameQuery.make(~gameId, ()), [|gameId|], data =>
      {
        id: data##game##id,
        name: data##game##name,
        logo: data##game##logo,
        sprites: data##game##sprites |> Array.map(sprite => {name: sprite##name, SpriteList.fileId: sprite##image}) |> Array.to_list,
      }
    );

  switch (game) {
  | NotLoaded => <div />
  | Loading => <span> {ReasonReact.string("Loading...")} </span>
  | Loaded(game) =>
    let logo = game.logo->Belt.Option.mapWithDefault(<> </>, logo => <img src={Environment.storageUrl ++ logo} width="64" height="64" />);

    <div>
      logo
      <span> {ReasonReact.string("Details of game with id: " ++ game.id)} </span>
      <button onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/new-match")}> {ReasonReact.string("New match")} </button>
      <button onClick={_ => ReasonReactRouter.push("/games/" ++ game.id ++ "/matches")}> {ReasonReact.string("See matches")} </button>
      <SpriteList uploadedSprites={game.sprites} canAdd=false />
    </div>;
  | Failure => <Translation id="common.error" />
  };
};