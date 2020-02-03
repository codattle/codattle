type _match = {
  id: string,
  name: string,
  scriptsCount: int,
  winner: option(int),
};

type x = {
  matches: list(_match),
  maxCountOfScripts: int,
};

module GetMatchesQuery = [%graphql
  {|
  query($gameId: ID!) {
    matches(gameId: $gameId) {
      id
      name
      scriptsCount
      result {
        winner
      }
    }
    game(gameId: $gameId) {
      maxAllowedPlayerCount
    }
  }
|}
];

module Styles = {
  open Css;
  open Style.Color;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
  let refreshButton = style([marginBottom(20 |> px)]);
  let item =
    style([
      display(`flex),
      alignItems(`center),
      height(30 |> px),
      marginTop(10 |> px),
      marginBottom(10 |> px),
      padding(10 |> px),
      backgroundColor(lightgray),
      cursor(`pointer),
      hover([backgroundColor(lightgray |> darken(0.2))]),
    ]);
  let itemTitle = style([fontWeight(`bold), overflow(`hidden), whiteSpace(`nowrap), textOverflow(`ellipsis)]);
};

[@react.component]
let make = (~gameId) => {
  let (version, refresh) = Utils.useRefresh();

  let queryResult =
    Utils.useResource(GetMatchesQuery.make(~gameId, ()), [|gameId, version|], data =>
      {
        matches:
          data##matches
          |> Array.to_list
          |> List.map(match =>
               {
                 id: match##id,
                 name: match##name,
                 scriptsCount: match##scriptsCount,
                 winner: Belt.Option.mapWithDefault(match##result, None, result => result##winner),
               }
             ),
        maxCountOfScripts: data##game##maxAllowedPlayerCount,
      }
    );

  let getMatchDescription = (~maxCountOfScripts, ~_match) => {
    <div>
      {ReasonReact.string(_match.name ++ " (" ++ string_of_int(_match.scriptsCount) ++ "/" ++ string_of_int(maxCountOfScripts) ++ ")")}
      <WinnerComponent winner={_match.winner} />
    </div>;
  };

  queryResult->Utils.displayResource(({matches, maxCountOfScripts}) => {
    let matchList =
      if (List.length(matches) == 0) {
        <Translation id="matchList.noMatches" />;
      } else {
        <div>
          {matches
           |> Utils.componentList(match =>
                <div className=Styles.item key={match.id} onClick={_ => ReasonReactRouter.push("/games/matches/" ++ match.id)}>
                  <span className=Styles.itemTitle> {getMatchDescription(~maxCountOfScripts, ~_match=match)} </span>
                </div>
              )}
        </div>;
      };
    <div className=Styles.container>
      <div className=Styles.refreshButton> <Button label="common.refresh" onClick={_ => refresh()} /> </div>
      matchList
    </div>;
  });
};