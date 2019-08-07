module GetMatchesQuery = [%graphql {|
  query {
    games {
      id
      name
    }
  }
|}];

[@react.component]
let make = (~gameId) =>
  <div>
    <span> {ReasonReact.string("Details of game with id: " ++ gameId)} </span>
    <button onClick={_ => ReasonReactRouter.push("/games/" ++ gameId ++ "/new-match")}> {ReasonReact.string("New match")} </button>
    <button onClick={_ => ReasonReactRouter.push("/games/" ++ gameId ++ "/matches")}> {ReasonReact.string("See matches")} </button>
  </div>;