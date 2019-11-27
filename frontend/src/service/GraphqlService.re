open Rationale.Function.Infix;

type query('a) = {
  .
  "parse": Js.Json.t => 'a,
  "query": string,
  "variables": Js.Json.t,
};

let executeQuery = (grapqhQuery: query('a)): PromiseUtils.t('a) =>
  {
    // TODO: Hook cannot be used here
    let (keycloak, _) = Keycloak.useKeycloak();
    Fetch.fetchWithInit(
      Environment.graphqlUrl,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Js.Dict.fromList([("query", Js.Json.string(grapqhQuery##query)), ("variables", grapqhQuery##variables)])
          |> Js.Json.object_
          |> Js.Json.stringify
          |> Fetch.BodyInit.make,
        ~headers=
          Fetch.HeadersInit.makeWithArray([|
            ("content-type", "application/json"),
            ("Authorization", "Bearer " ++ (keycloak |> Keycloak.token)),
          |]),
        (),
      ),
    );
  }
  |> FetchUtils.parseJson
  |> PromiseUtils.flatMapOk(
       Js.Json.decodeObject
       ||> Rationale.Option.flatMap(Js.Dict.get(_, "data"))
       ||> Rationale.Option.flatMap(Js.Json.decodeObject)
       ||> Rationale.Option.map(Js.Json.object_)
       ||> Rationale.Option.map(grapqhQuery##parse)
       ||> Rationale.Result.ofOption(),
     );