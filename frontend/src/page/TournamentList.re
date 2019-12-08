type tournament = {
  id: string,
  name: string,
  scriptCount: int,
  maxScriptCount: int,
};

module GetTournamentsQuery = [%graphql
  {|
  query($gameId: ID!) {
    tournaments(gameId: $gameId) {
      id
      name
      scripts {
        id
      }
      maxScriptCount
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

  let tournaments =
    Utils.useResource(GetTournamentsQuery.make(~gameId, ()), [|gameId, version|], data =>
      data##tournaments
      |> Array.to_list
      |> List.map(tournament =>
           {
             id: tournament##id,
             name: tournament##name,
             scriptCount: tournament##scripts |> Array.length,
             maxScriptCount: tournament##maxScriptCount,
           }
         )
    );

  tournaments->Utils.displayResource(tournaments => {
    let tournamentList =
      if (List.length(tournaments) == 0) {
        <Translation id="tournamentList.noTournaments" />;
      } else {
        <div>
          {tournaments
           |> Utils.componentList(tournament =>
                <div
                  key={tournament.id} className=Styles.item onClick={_ => ReasonReactRouter.push("/games/tournaments/" ++ tournament.id)}>
                  <div className=Styles.itemTitle>
                    {ReasonReact.string(
                       tournament.name
                       ++ " ("
                       ++ string_of_int(tournament.scriptCount)
                       ++ "/"
                       ++ string_of_int(tournament.maxScriptCount)
                       ++ ")",
                     )}
                  </div>
                </div>
              )}
        </div>;
      };
    <div className=Styles.container> <Button label="common.refresh" onClick={_ => refresh()}> </Button> tournamentList </div>;
  });
};