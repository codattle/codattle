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
           | Some(_) => showNotification("scriptDetails.notification.updateSuccessfully", `Success)
           | None => showNotification("scriptDetails.notification.errorDuringUpdate", `Error)
           }
         )
    | _ => ()
    };
  };

  <div>
    {switch (script) {
     | NotLoaded => <> </>
     | Loading => <Translation id="common.loading" />
     | Loaded({code}) =>
       <div>
         <ScriptEditor value=code onChange={code => setScript(_ => {code: code})} />
         <Button label="scriptDetails.update" onClick=updateScriptCode />
       </div>
     | Failure => <Translation id="common.error" />
     }}
  </div>;
};