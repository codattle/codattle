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

[@react.component]
let make = () => {
  open SearchList;

  let search = ({name}: filters) => SearchGamesQuery.make(~name, ());
  let queryMapper = data => data##games |> Array.to_list |> List.map((game) => ({id: game##id, name: game##name}: game));
  let displayMapper = ({id, name}: game) => {id, name};
  let onItemClick = ({id}: game) => ReasonReactRouter.push("/games/" ++ id);

  <SearchList search queryMapper displayMapper onItemClick />;
};