type query('a) = {
  .
  "parse": Js.Json.t => 'a,
  "query": string,
  "variables": Js.Json.t,
};

let executeQuery = (grapqhQuery: query('a)) =>
  Fetch.fetchWithInit(
    Environment.graphqlUrl,
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=
        Js.Dict.fromList([("query", Js.Json.string(grapqhQuery##query)), ("variables", grapqhQuery##variables)])
        |> Js.Json.object_
        |> Js.Json.stringify
        |> Fetch.BodyInit.make,
      ~headers=Fetch.HeadersInit.makeWithArray([|("content-type", "application/json")|]),
      (),
    ),
  )
  |> Repromise.Rejectable.fromJsPromise
  |> Repromise.Rejectable.andThen(response =>
       if (Fetch.Response.ok(response)) {
         Fetch.Response.json(response) |> Repromise.Rejectable.fromJsPromise |> Repromise.Rejectable.map(json => Some(json));
       } else {
         Repromise.Rejectable.resolved(None);
       }
     )
  |> Repromise.Rejectable.map(optionalJson =>
       switch (optionalJson) {
       | Some(json) =>
         switch (Js.Json.decodeObject(json)) {
         | Some(obj) => Some(Js.Dict.unsafeGet(obj, "data") |> grapqhQuery##parse)
         | None => None
         }
       | None => None
       }
     )
  |> Repromise.Rejectable.catch(_error => Repromise.resolved(None));