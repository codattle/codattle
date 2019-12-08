type script = {
  id: string,
  code: string,
};

module GetScriptsQuery = [%graphql
  {|
  query($gameId: ID, $authorId: ID) {
    scripts(gameId: $gameId, authorId: $authorId) {
      id
      code
    }
  }
|}
];

module Styles = {
  open Css;
  open Style.Color;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
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
  let scripts =
    Utils.useResource(GetScriptsQuery.make(~gameId, ~authorId=ProfileService.userId, ()), [|gameId, ProfileService.userId|], data =>
      data##scripts |> Array.to_list |> List.map(script => {id: script##id, code: script##code})
    );

  scripts->Utils.displayResource(scripts =>
    switch (scripts) {
    | [] => <Translation id="myScripts.noScripts" />
    | scripts =>
      <div className=Styles.container>
        {scripts
         |> Utils.componentListWithIndex((index, script) =>
              <div className=Styles.item key={script.id} onClick={_ => ReasonReactRouter.push("/games/scripts/" ++ script.id)}>
                <span className=Styles.itemTitle> {ReasonReact.string("#" ++ string_of_int(index + 1))} </span>
              </div>
            )}
      </div>
    }
  );
};