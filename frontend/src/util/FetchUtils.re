let parseJson = (response: Js.Promise.t(Fetch.Response.t)): PromiseUtils.t(Js.Json.t) =>
  response
  |> PromiseUtils.fromJsPromise
  |> Repromise.andThenOk(response =>
       if (Fetch.Response.ok(response)) {
         Fetch.Response.json(response) |> PromiseUtils.fromJsPromise;
       } else {
         Repromise.resolved(Belt.Result.Error());
       }
     );