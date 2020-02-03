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

module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto)]);
  let updateButton = style([marginBottom(20 |> px)]);
  let codeEditor = style([height(600 |> px)]);
};

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
      ->Promise.get(result =>
           switch (result) {
           | Belt.Result.Ok(_) => showNotification("common.notification.updateSuccessfully", `Success)
           | Error(_) => showNotification("common.notification.errorDuringUpdate", `Error)
           }
         )
    | _ => ()
    };
  };

  script->Utils.displayResource(({code}) =>
    <div className=Styles.container>
      <div className=Styles.updateButton> <Button label="scriptDetails.update" onClick=updateScriptCode /> </div>
      <div className=Styles.codeEditor> <CodeEditor value=code onChange={code => setScript(_ => {code: code})} /> </div>
    </div>
  );
};