open Rationale.Option.Infix;
open OptionUtils.Infix;

type script = {
  id: string,
  index: int,
  code: string,
  newCode: string,
};

type matchResult = {
  winner: option(int),
  frames: option(Selector.Required.t(MatchResult.frame)),
};

type loadedState = {
  gameDescription: I18nText.t,
  scripts: Selector.Optional.t(script),
  scriptSaving: bool,
  players: list(Selector.Optional.t(script)),
  matchFrameContext: MatchFrame.context,
  matchCreating: bool,
  matchId: option(string),
  matchResult: option(matchResult),
};

type state = Utils.resource(loadedState);

type game = {
  description: I18nText.t,
  playerCount: int,
  scripts: list(script),
  matchFrameContext: MatchFrame.context,
};

type savedScript = {
  id: string,
  code: string,
};

type action =
  | Load(game)
  | AddScript(script)
  | SetSelectedScriptCode(string)
  | SelectScript(Selector.Optional.t(script))
  | StartSavingScript
  | StopSavingScript(option(savedScript))
  | SelectPlayer(Selector.Optional.t(script), int)
  | StartCreatingMatch
  | StopCreatingMatch(option(string))
  | LoadMatchResult(option(matchResult))
  | SelectFrame(Selector.Required.t(MatchResult.frame))
  | Fail;

module GetGameQuery = [%graphql
  {|
  query($gameId: ID!) {
    game(gameId: $gameId) {
      description {
        language
        content
      }
      scripts {
        id
        code
      }
      sprites {
          name
          image {
            id
          }
        }
      maxAllowedPlayerCount
    }
  }
|}
];

module GetMatchQuery = [%graphql
  {|
  query($matchId: ID!) {
    match_: match(matchId: $matchId) {
      result {
        winner
        resultFrames {
          order
          content
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

module UpdateScriptCodeMutation = [%graphql
  {|
  mutation($scriptId: ID!, $code: String!) {
    updateScriptCode(scriptId: $scriptId, code: $code)
  }
|}
];

module CreateInstantMatchMutation = [%graphql
  {|
  mutation($gameId: ID!, $scripts: [ID!]!) {
    createInstantMatch(gameId: $gameId, scripts: $scripts) {
      id
    }
  }
|}
];

module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
  let actionMenu = style([display(`flex), marginTop(20 |> px), marginBottom(20 |> px), children([marginRight(20 |> px)])]);
  let section = style([marginTop(20 |> px), marginBottom(20 |> px)]);
  let scriptSelectors = style([display(`flex), margin4(~top=20 |> px, ~bottom=20 |> px, ~left=(-10) |> px, ~right=0 |> px)]);
  let scriptSelector = style([margin2(~v=0 |> px, ~h=10 |> px)]);
  let header = style([fontSize(16 |> px), fontWeight(`bold), marginTop(40 |> px)]);
};

let scriptNameMapper = ({index, code, newCode}: script): React.element =>
  React.string("#" ++ string_of_int(index) ++ (code === newCode ? "" : "*"));

[@react.component]
let make = (~gameId: string) => {
  let (state, send) =
    React.useReducer(
      (state: state, action) =>
        switch (action) {
        | Load({description, playerCount, scripts, matchFrameContext}) =>
          Loaded({
            gameDescription: description,
            scripts: scripts |> Selector.Optional.fromListWithFirstSelected,
            scriptSaving: false,
            players: Rationale.RList.create(_ => scripts |> Selector.Optional.fromList, playerCount),
            matchFrameContext,
            matchCreating: false,
            matchId: None,
            matchResult: None,
          })
        | Fail => Failure
        | _ =>
          switch (state) {
          | Loaded({scripts, players, matchResult} as state) =>
            switch (action) {
            | AddScript(script) =>
              Loaded({
                ...state,
                scripts:
                  scripts
                  |> Selector.Optional.toList
                  |> ((list) => ({before: list, selected: Some(script), after: []}: Selector.Optional.t(script))),
                players: players |> List.map(Selector.addLast(script)),
              })
            | SetSelectedScriptCode(code) =>
              Loaded({...state, scripts: scripts |> Selector.Optional.mapSelected(script => {...script, newCode: code})})
            | SelectScript(scripts) => Loaded({...state, scripts})
            | StartSavingScript => Loaded({...state, scriptSaving: true})
            | StopSavingScript(savedScript) =>
              Loaded({
                ...state,
                scripts:
                  savedScript
                  <$> (({id, code}) => scripts |> Selector.Optional.mapFirst((x: script) => x.id === id, x => {...x, code}))
                  ||? scripts,
                scriptSaving: false,
              })
            | SelectPlayer(scriptSelector, playerIndex) =>
              Loaded({...state, players: players |> Rationale.RList.update(scriptSelector, playerIndex)})
            | StartCreatingMatch => Loaded({...state, matchCreating: true})
            | StopCreatingMatch(matchId) => Loaded({...state, matchCreating: false, matchId})
            | LoadMatchResult(matchResult) => Loaded({...state, matchResult})
            | SelectFrame(frames) =>
              Loaded({
                ...state,
                matchResult: {
                  matchResult <$> (({winner}) => {winner, frames: Some(frames)});
                },
              })
            | _ => Loaded(state)
            }
          | _ => state
          }
        },
      Loading,
    );
  React.useEffect1(
    () => {
      let callback = result =>
        switch (result) {
        | Belt.Result.Ok(data) => send(Load(data))
        | Error(_) => send(Fail)
        };
      let mapper = data => {
        description: data##game##description |> I18nText.fromJs,
        playerCount: data##game##maxAllowedPlayerCount,
        scripts:
          data##game##scripts
          |> ArrayUtils.mapiToList((index, script) => {id: script##id, index, code: script##code, newCode: script##code}),
        matchFrameContext:
          MatchFrame.{
            fileIdBySpriteName:
              data##game##sprites |> Array.map(sprite => (sprite##name, sprite##image##id)) |> Belt.Map.String.fromArray,
          },
      };
      Some(Utils.loadResource(GetGameQuery.make(~gameId, ()), callback, mapper));
    },
    [|gameId|],
  );

  let matchId =
    switch (state) {
    | Loaded({matchId}) => matchId
    | _ => None
    };
  let (matchResultVersion, refreshMatchResult) = Utils.useRefresh();
  React.useEffect1(
    () =>
      switch (matchId) {
      | Some(matchId) =>
        let callback = result =>
          switch (result) {
          | Belt.Result.Ok(data) => send(LoadMatchResult(data))
          | Error(_) => send(Fail)
          };
        let mapper = data =>
          data##match##result <$> (result => {winner: result##winner, frames: result##resultFrames |> MatchResult.parseResultFrames});
        Some(Utils.loadResource(GetMatchQuery.make(~matchId, ()), callback, mapper));
      | None => None
      },
    [|matchId, Some(matchResultVersion)|],
  );

  state->Utils.displayResource(({gameDescription, scripts, scriptSaving, players, matchCreating, matchResult, matchFrameContext}) => {
    let isSelectedScriptCodeChanged = scripts.selected <$> (({code, newCode}) => code !== newCode) ||? false;
    let saveSelectedScriptCode = () =>
      scripts.selected
      <$> (
        ({id, newCode}) => {
          send(StartSavingScript);
          GraphqlService.executeQuery(UpdateScriptCodeMutation.make(~scriptId=id, ~code=newCode, ()))
          |> Repromise.wait(result => send(StopSavingScript(Rationale.Result.isOk(result) ? Some({id, code: newCode}) : None)));
        }
      )
      |> ignore;
    let addNewScript = () =>
      GraphqlService.executeQuery(SendScriptMutation.make(~gameId, ~code="", ()))
      |> Repromise.wait(result =>
           switch (result) {
           | Belt.Result.Ok(data) =>
             send(AddScript({id: data##sendScript##id, index: scripts |> Selector.Optional.length, code: "", newCode: ""}))
           | Error(_) => ()
           }
         );
    let createInstantMatch = () =>
      players
      |> List.map(Selector.selected)
      |> List.fold_left((list, selected) => selected <$> Rationale.RList.append <*> list, Some([]))
      <$> List.map((x: script) => x.id)
      <$> Array.of_list
      <$> CreateInstantMatchMutation.make(~gameId, ~scripts=_, ())
      <$> {
        send(StartCreatingMatch);
        GraphqlService.executeQuery(_);
      }
      <$> Repromise.wait(result =>
            send(StopCreatingMatch(result |> Rationale.Option.ofResult <$> (data => data##createInstantMatch##id)))
          )
      |> ignore;
    let canCreateInstantMatch = players |> List.map(Selector.selected) |> Rationale.RList.none(Rationale.Option.isNone);

    let codeEditor =
      scripts.selected
      <$> (script => <CodeEditor value={script.newCode} onChange={code => send(SetSelectedScriptCode(code))} />)
      ||? <> </>;
    let playerSelectors =
      <div className=Styles.scriptSelectors>
        {players
         |> Utils.componentListWithIndex((i, player) =>
              <div key={string_of_int(i)} className=Styles.scriptSelector>
                <Select value=player nameMapper=scriptNameMapper onChange={newPlayer => send(SelectPlayer(newPlayer, i))} />
              </div>
            )}
      </div>;
    let result =
      matchResult
      <$> (
        ({frames, winner}) => <MatchResult frames winner context=matchFrameContext onChange={frames => send(SelectFrame(frames))} />
      )
      ||? <> </>;
    let refreshResultButton =
      Rationale.Option.isSome(matchId) ? <Button label="scriptTesting.refresh" onClick=refreshMatchResult /> : <> </>;

    <div className=Styles.container>
      <GameDescription description=gameDescription />
      <div className=Styles.actionMenu>
        <div> <Button label="scriptTesting.newScript" onClick=addNewScript /> </div>
        <div>
          <Button label="scriptTesting.save" disabled={!isSelectedScriptCodeChanged || scriptSaving} onClick=saveSelectedScriptCode />
        </div>
      </div>
      <div className=Styles.section>
        <Select value=scripts nameMapper=scriptNameMapper onChange={scripts => send(SelectScript(scripts))} />
      </div>
      codeEditor
      <div className=Styles.header> <Translation id="scriptTesting.chooseScriptsToTest" /> </div>
      <div> playerSelectors </div>
      <div className=Styles.actionMenu>
        <div> <Button label="scriptTesting.test" disabled={!canCreateInstantMatch || matchCreating} onClick=createInstantMatch /> </div>
        refreshResultButton
      </div>
      result
    </div>;
  });
};