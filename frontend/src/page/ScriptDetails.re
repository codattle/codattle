type script = {code: string};

module GetScriptQuery = [%graphql {|
  query($scriptId: ID!) {
    script(scriptId: $scriptId) {
      code
    }
  }
|}];

module UpdateScriptCodeMutation = [%graphql
  {|
  mutation($scriptId: ID!, $code: String!) {
    updateScriptCode(scriptId: $scriptId, code: $code)
  }
|}
];

[@react.component]
let make = (~scriptId) => {
  open Notifications;

  let (script, setScript) =
    Utils.useEditableResource(GetScriptQuery.make(~scriptId, ()), [|scriptId|], data => {code: data##script##code});
  let {showNotification} = useNotifications();

  let updateScriptCode = () => {
    switch (script) {
    | Loaded(script) =>
      GraphqlService.executeQuery(UpdateScriptCodeMutation.make(~scriptId, ~code=script.code, ()))
      |> Repromise.wait(result =>
           switch (result) {
           | Belt.Result.Ok(_) => showNotification("common.notification.updateSuccessfully", `Success)
           | Error(_) => showNotification("common.notification.errorDuringUpdate", `Error)
           }
         )
    | _ => ()
    };
  };

  script->Utils.displayResource(({code}) =>
    <div>
      <ScriptEditor value=code onChange={code => setScript(_ => {code: code})} />
      <Button label="scriptDetails.update" onClick=updateScriptCode />
    </div>
  );
};