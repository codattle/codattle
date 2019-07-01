type match = {
  id: string,
  winner: option(int),
};

module GetMatchQuery = [%graphql{|
  query($matchId: ID!) {
    match_: match(matchId: $matchId) {
      game {
        id
      }
      result {
        winner
      }
    }
  }
|}];

[@react.component]
let make = (~matchId) => {
  let match =
    Utils.useResource(GetMatchQuery.make(~matchId, ()), [|matchId|], data =>
      {id: data##match##game##id, winner: data##match##result->Belt.Option.flatMap(result => result##winner)}
    );

  switch (match) {
  | NotLoaded => <div />
  | Loading => <div> {ReasonReact.string("Loading...")} </div>
  | Loaded(match) =>
    <div>
      <div> {ReasonReact.string("Details of match with id: " ++ matchId)} </div>
      {match.winner
       ->Belt.Option.mapWithDefault(<> </>, winner => <div> {ReasonReact.string("Winner: " ++ string_of_int(winner + 1))} </div>)}
      <button onClick={_ => ReasonReactRouter.push("/games/matches/" ++ matchId ++ "/new-script")}>
        {ReasonReact.string("New script")}
      </button>
    </div>
  | Failure => <div> {ReasonReact.string("Failure while loading match :(")} </div>
  };
};