open Rationale.Option.Infix;

type game = {
  id: string,
  description: I18nText.t,
};

type mode =
  | SelectingScript
  | Joining
  | Failure;

type state = {
  mode,
  game,
  scriptSelector: ScriptSelector.t,
};

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

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
  let section = style([marginBottom(20 |> px)]);
};

[@react.component]
let make = (~matchId: string) => {
  let (state, setState) =
    Utils.useEditableResource(GetMatchQuery.make(~matchId, ~userId=ProfileService.userId, ()), [|matchId, ProfileService.userId|], data =>
      {
        mode: SelectingScript,
        game: {
          id: data##match##game##id,
          description: data##match##game##description |> I18nText.fromJs,
        },
        scriptSelector: {
          newScriptCode: "",
          scripts:
            data##match##game##scripts
            |> ArrayUtils.mapToList((script) => ({id: script##id, code: script##code}: ScriptSelector.script))
            |> Selector.Optional.fromList,
        },
      }
    );

  state->Utils.displayResource(({mode, game, scriptSelector}) =>
    switch (mode) {
    | SelectingScript =>
      let sendScript = () =>
        GraphqlService.executeQuery(SendScriptMutation.make(~gameId=game.id, ~code=scriptSelector.newScriptCode, ()))
        |> Repromise.mapOk(result => result##sendScript##id);
      let joinMatch = () => {
        setState(state => {...state, mode: Joining});
        scriptSelector.scripts.selected
        <$> (({id}) => id)
        <$> PromiseUtils.resolved
        |> OptionUtils.default(sendScript)
        |> Repromise.andThenOk(scriptId => GraphqlService.executeQuery(JoinMatchMutation.make(~matchId, ~scriptId, ())))
        |> Repromise.wait(result =>
             switch (result) {
             | Belt.Result.Ok(_) => ReasonReactRouter.push("/games/matches/" ++ matchId)
             | Error(_) => setState(state => {...state, mode: Failure})
             }
           );
      };

      <div className=Styles.container>
        <div className=Styles.section> <GameDescription description={game.description} /> </div>
        <div className=Styles.section>
          <ScriptSelector value=scriptSelector onChange={scriptSelector => setState(state => {...state, scriptSelector})} />
        </div>
        <div className=Styles.section> <Button label="scriptWizard.joinMatch" onClick=joinMatch /> </div>
      </div>;
    | Joining => <div> <Translation id="scriptWizard.joiningMatch" /> </div>
    | Failure => <div> <Translation id="common.error" /> </div>
    }
  );
};