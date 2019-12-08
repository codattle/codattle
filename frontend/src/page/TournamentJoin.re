open Rationale.Option.Infix;

type mode =
  | SelectingScript
  | Joining
  | Failure;

type state = {
  mode,
  gameId: string,
  scriptSelector: ScriptSelector.t,
};

module GetTournamentQuery = [%graphql
  {|
  query($tournamentId: ID!, $userId: ID!) {
    tournament(tournamentId: $tournamentId) {
      game {
        id
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

module JoinTournamentMutation = [%graphql
  {|
  mutation($tournamentId: ID!, $scriptId: ID!) {
    joinTournament(tournamentId: $tournamentId, scriptId: $scriptId)
  }
|}
];

module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
  let section = style([marginBottom(20 |> px)]);
};

[@react.component]
let make = (~tournamentId: string) => {
  let (state, setState) =
    Utils.useEditableResource(
      GetTournamentQuery.make(~tournamentId, ~userId=ProfileService.userId, ()), [|tournamentId, ProfileService.userId|], data =>
      {
        mode: SelectingScript,
        gameId: data##tournament##game##id,
        scriptSelector: {
          newScriptCode: "",
          scripts:
            data##tournament##game##scripts
            |> ArrayUtils.mapToList((script) => ({id: script##id, code: script##code}: ScriptSelector.script))
            |> Selector.Optional.fromList,
        },
      }
    );

  state->Utils.displayResource(({mode, gameId, scriptSelector}) =>
    switch (mode) {
    | SelectingScript =>
      let sendScript = () =>
        GraphqlService.executeQuery(SendScriptMutation.make(~gameId, ~code=scriptSelector.newScriptCode, ()))
        |> Repromise.mapOk(result => result##sendScript##id);
      let joinTournament = () => {
        setState(state => {...state, mode: Joining});
        scriptSelector.scripts.selected
        <$> (({id}) => id)
        <$> PromiseUtils.resolved
        |> OptionUtils.default(sendScript)
        |> Repromise.andThenOk(scriptId => GraphqlService.executeQuery(JoinTournamentMutation.make(~tournamentId, ~scriptId, ())))
        |> Repromise.wait(result =>
             switch (result) {
             | Belt.Result.Ok(_) => ReasonReactRouter.push("/games/tournaments/" ++ tournamentId)
             | Error(_) => setState(state => {...state, mode: Failure})
             }
           );
      };

      <div className=Styles.container>
        <div className=Styles.section>
          <ScriptSelector value=scriptSelector onChange={scriptSelector => setState(state => {...state, scriptSelector})} />
        </div>
        <Button label="tournamentJoin.joinToTournament" onClick=joinTournament />
      </div>;
    | Joining => <div> <Translation id="tournamentJoin.joiningToTournament" /> </div>
    | Failure => <div> <Translation id="common.error" /> </div>
    }
  );
};