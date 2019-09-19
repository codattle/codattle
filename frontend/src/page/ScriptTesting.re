open Rationale.Option.Infix;
open OptionUtils.Infix;

type script = {
  id: string,
  index: int,
  code: string,
  newCode: string,
};

type state = {
  gameDescription: I18nText.t,
  scripts: Selector.Optional.t(script),
  scriptSaving: bool,
};

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

let scriptNameMapper = ({index, code, newCode}: script): React.element =>
  React.string("#" ++ string_of_int(index) ++ (code === newCode ? "" : "*"));

[@react.component]
let make = (~gameId: string) => {
  let (state, setState) =
    Utils.useEditableResource(GetGameQuery.make(~gameId, ()), [|gameId|], data =>
      {
        gameDescription: data##game##description |> I18nText.fromJs,
        scripts:
          data##game##scripts
          |> ArrayUtils.mapiToList((index, script) => {id: script##id, index, code: script##code, newCode: script##code})
          |> Selector.Optional.fromListWithFirstSelected,
        scriptSaving: false,
      }
    );
  let changeSelectedScriptCode = (code: string): unit =>
    setState(state => {...state, scripts: state.scripts |> Selector.Optional.mapSelected(script => {...script, newCode: code})});

  state->Utils.displayResource(({gameDescription, scripts, scriptSaving}) => {
    let isSelectedScriptCodeUnchanged = scripts.selected <$> (({code, newCode}) => code === newCode) ||? false;
    let saveSelectedScriptCode = () =>
      scripts.selected
      <$> (
        ({id, newCode}) => {
          setState(state => {...state, scriptSaving: true});
          GraphqlService.executeQuery(UpdateScriptCodeMutation.make(~scriptId=id, ~code=newCode, ()))
          |> Repromise.wait(result =>
               switch (result) {
               | Belt.Result.Ok(_) =>
                 setState(state =>
                   {
                     ...state,
                     scriptSaving: false,
                     scripts: scripts |> Selector.Optional.mapFirst(script => script.id === id, script => {...script, code: newCode}),
                   }
                 )
               | Error(_) => setState(state => {...state, scriptSaving: false})
               }
             );
        }
      )
      |> ignore;
    let addNewScript = () =>
      GraphqlService.executeQuery(SendScriptMutation.make(~gameId, ~code="", ()))
      |> Repromise.wait(result =>
           switch (result) {
           | Belt.Result.Ok(data) =>
             let newScript = {id: data##sendScript##id, index: scripts |> Selector.Optional.length, code: "", newCode: ""};
             setState(state =>
               {
                 ...state,
                 scripts:
                   scripts
                   |> Selector.Optional.all
                   |> ((scripts) => ({before: scripts, selected: Some(newScript), after: []}: Selector.Optional.t(script))),
               }
             );
           | Error(_) => ()
           }
         );

    <div>
      <GameDescription description=gameDescription />
      <div> <Button label="scriptTesting.newScript" onClick=addNewScript /> </div>
      <div> <Select value=scripts nameMapper=scriptNameMapper onChange={scripts => setState(state => {...state, scripts})} /> </div>
      {scripts.selected <$> (script => <CodeEditor value={script.newCode} onChange=changeSelectedScriptCode />) ||? <> </>}
      <div>
        <Button label="scriptTesting.save" disabled={isSelectedScriptCodeUnchanged && !scriptSaving} onClick=saveSelectedScriptCode />
      </div>
    </div>;
  });
};