open Rationale.Option;
open Rationale.Function;

type script = {
  id: option(string),
  name: React.element,
  code: string,
};

type game = {
  id: string,
  description: I18nText.t,
  scripts: list(script),
};

type selectingScript = {
  newScript: script,
  selectedScript: option(script),
};

type state =
  | SelectingScript(selectingScript)
  | Joining
  | Failure;

module GetMatchQuery = [%graphql
  {|
  query($matchId: ID!, $userId: ID!) {
    match_: match(matchId: $matchId) {
      game {
        id
        description {
          language
          content
        }
        scripts(authorId: $userId) {
          id
          code
        }
      }
    }
  }
|}
];

module SendScriptMutation = [%graphql
  {|
  mutation($gameId: ID!, $code: String!) {
    sendScript(gameId: $gameId, code: $code) {
      id
    }
  }
|}
];

module JoinMatchMutation = [%graphql
  {|
  mutation($matchId: ID!, $scriptId: ID!) {
    joinMatch(matchId: $matchId, scriptId: $scriptId)
  }
|}
];

module Styles = {
  open Css;

  let section = style([marginTop(15 |> px), marginBottom(15 |> px)]);
};

let useGame = (~matchId) =>
  Utils.useResource(GetMatchQuery.make(~matchId, ~userId=ProfileService.userId, ()), [|matchId, ProfileService.userId|], data =>
    {
      id: data##match##game##id,
      description: data##match##game##description |> I18nText.fromJs,
      scripts:
        data##match##game##scripts
        |> Array.mapi((index, script) => {id: Some(script##id), name: React.string("#" ++ string_of_int(index)), code: script##code})
        |> Array.to_list,
    }
  );

let sendNewScript = (~gameId, ~code) =>
  GraphqlService.executeQuery(SendScriptMutation.make(~gameId, ~code, ())) |> Repromise.map(fmap(result => result##sendScript##id));

[@react.component]
let make = (~matchId: string) => {
  let (state, setState) =
    React.useState(() =>
      SelectingScript({
        newScript: {
          id: None,
          name: <Translation id="scriptWizard.newScript" />,
          code: "",
        },
        selectedScript: None,
      })
    );
  let game = useGame(~matchId);

  game->Utils.displayResource(game =>
    switch (state) {
    | SelectingScript({newScript, selectedScript}) =>
      let script = selectedScript |> default(newScript);
      let selectExistentScript = selectedScript => setState(_ => SelectingScript({newScript, selectedScript: Some(selectedScript)}));
      let selectNewScript = code => setState(_ => SelectingScript({
                                                    newScript: {
                                                      ...newScript,
                                                      code,
                                                    },
                                                    selectedScript: None,
                                                  }));
      let joinMatch = (script: script) => {
        setState(_ => Joining);
        let scriptId =
          switch (script) {
          | {id: Some(scriptId)} => Repromise.resolved(Some(scriptId))
          | {code} => sendNewScript(~gameId=game.id, ~code)
          };
        scriptId
        |> Repromise.andThen(
             fmap(scriptId => GraphqlService.executeQuery(JoinMatchMutation.make(~matchId, ~scriptId, ())))
             ||> default(Repromise.resolved(None)),
           )
        |> Repromise.wait(result =>
             switch (result) {
             | Some(_) => ReasonReactRouter.push("/games/matches/" ++ matchId)
             | None => setState(_ => Failure)
             }
           );
      };

      Styles.(
        <div className=section>
          <div className=section> <GameDescription description={game.description} /> </div>
          <div className=section>
            <Select value=script items=[newScript, ...game.scripts] itemMapper={script => script.name} onChange=selectExistentScript />
          </div>
          <div className=section> <ScriptEditor value={script.code} onChange=selectNewScript /> </div>
          <Button label="scriptWizard.joinMatch" onClick={_ => joinMatch(script)} />
        </div>
      );
    | Joining => <div> <Translation id="scriptWizard.joiningMatch" /> </div>
    | Failure => <div> <Translation id="common.error" /> </div>
    }
  );
};