let parseJson = (response: Js.Promise.t(Fetch.Response.t)): PromiseUtils.t(Js.Json.t) =>
  response
  ->PromiseUtils.fromJsPromise
  ->Promise.flatMapOk(response =>
       if (Fetch.Response.ok(response)) {
         Fetch.Response.json(response)->PromiseUtils.fromJsPromise
       } else {
         Promise.resolved(Belt.Result.Error());
       }
     );