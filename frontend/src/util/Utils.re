type resource('a) =
  | NotLoaded
  | Loading
  | Loaded('a)
  | Failure;

let loadResource = (query, setter, mapper) => {
  setter(_ => Loading);
  GraphqlService.executeQuery(query)
  |> Repromise.wait(result =>
       switch (result) {
       | Some(data) => setter(_ => Loaded(mapper(data)))
       | None => setter(_ => Failure)
       }
     );
};

let useResource = (query, shouldRefresh, mapper) => {
  let (resource, setResource) = React.useState(() => NotLoaded);

  React.useEffect1(
    () => {
      loadResource(query, setResource, mapper);
      // TODO: request should be unsubscribed using AbortController
      // https://developer.mozilla.org/en-US/docs/Web/API/AbortController
      None;
    },
    shouldRefresh,
  );

  resource;
};

let useRefresh = () => {
  let (version, setVersion) = React.useState(() => 0);

  let refresh = () => setVersion(currentVersion => currentVersion + 1);

  (string_of_int(version), refresh);
};

let ifSome = (action, item) => Belt.Option.mapWithDefault(item, (), action);

let componentList = (mapper, items) => items |> List.map(mapper) |> Array.of_list |> ReasonReact.array;