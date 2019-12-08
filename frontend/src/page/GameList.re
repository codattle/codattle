type game = {
  id: string,
  name: string,
};

module SearchGamesQuery = [%graphql {|
  query($name: String) {
    games: searchGames(name: $name) {
      id
      name
    }
  }
|}];

module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
};

[@react.component]
let make = () => {
  let search = ({name}: SearchList.filters) => SearchGamesQuery.make(~name, ());
  let queryMapper = data => data##games |> Array.to_list |> List.map((game) => ({id: game##id, name: game##name}: game));
  let displayMapper = ({id, name}: game): SearchList.display => {id, name};
  let onItemClick = ({id}: game) => ReasonReactRouter.push("/games/" ++ id);

  <div className=Styles.container> <SearchList search queryMapper displayMapper onItemClick /> </div>;
};