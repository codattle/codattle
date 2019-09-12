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

[@react.component]
let make = (~gameId) => {
  let scripts =
    Utils.useResource(GetScriptsQuery.make(~gameId, ~authorId=ProfileService.userId, ()), [|gameId, ProfileService.userId|], data =>
      data##scripts |> Array.to_list |> List.map(script => {id: script##id, code: script##code})
    );

  <div>
    {switch (scripts) {
     | NotLoaded => <> </>
     | Loading => <Translation id="common.loading" />
     | Loaded([]) => <Translation id="myScripts.noScripts" />
     | Loaded(scripts) =>
       <ul>
         {scripts
          |> Utils.componentListWithIndex((index, script) =>
               <li key={script.id} onClick={_ => ReasonReactRouter.push("/games/scripts/" ++ script.id)}>
                 {ReasonReact.string("#" ++ string_of_int(index + 1))}
               </li>
             )}
       </ul>
     | Failure => <Translation id="common.error" />
     }}
  </div>;
};