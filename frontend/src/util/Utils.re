open Rationale.Function.Infix;

type resource('a) =
  | Loading
  | Loaded('a)
  | Failure;

let useEffectWithInit = (initialize, effect, cancel, shouldRefresh) => {
  let (initialized, setInitialized) = React.useState(() => false);

  React.useEffect1(
    () => {
      if (initialized) {
        effect();
      } else {
        initialize();
        setInitialized(_ => true);
      };
      cancel;
    },
    shouldRefresh,
  );
};

let loadResource = (query: GraphqlService.query('a), callback: Belt.Result.t('b, unit) => unit, mapper: 'a => 'b): (unit => unit) => {
  GraphqlService.executeQuery(query)->Promise.get(Rationale.Result.fmap(mapper) ||> callback);
  // TODO: request should be unsubscribed using AbortController
  // https://developer.mozilla.org/en-US/docs/Web/API/AbortController
  () => ();
};

let mapResultToResourceSetter = (result: Belt.Result.t('a, unit)): (resource('a) => resource('a)) =>
  switch (result) {
  | Ok(data) => (_ => Loaded(data))
  | Error(_) => (_ => Failure)
  };

let useEditableResource = (query, shouldRefresh, mapper) => {
  let (resource, setResource) = React.useState(() => Loading);

  React.useEffect1(() => Some(loadResource(query, mapResultToResourceSetter ||> setResource, mapper)), shouldRefresh);

  (
    resource,
    newResourceProducer =>
      switch (resource) {
      | Loaded(loadedResource) => setResource(_ => Loaded(newResourceProducer(loadedResource)))
      | _ => ()
      },
  );
};

let useResource = (query, shouldRefresh, mapper) => {
  let (resource, _) = useEditableResource(query, shouldRefresh, mapper);
  resource;
};

let useResourceWithDebounce = (query, shouldRefresh, wait, mapper) => {
  let (resource, setResource) = React.useState(() => Loading);
  let (cancel, setCancel) = React.useState(() => None);
  let (loadResourceDebouncer, _) =
    React.useState(() =>
      Debouncer.makeCancelable(~wait, ((query, setResource, mapper)) => setCancel(_ => Some(loadResource(query, setResource, mapper))))
    );

  useEffectWithInit(
    () => loadResourceDebouncer.invoke((query, mapResultToResourceSetter ||> setResource, mapper)),
    () => loadResourceDebouncer.schedule((query, mapResultToResourceSetter ||> setResource, mapper)),
    Some(
      () => {
        cancel
        |> Rationale.Option.iter(cancel => {
             cancel();
             setCancel(_ => None);
           });
        loadResourceDebouncer.cancel();
      },
    ),
    shouldRefresh,
  );

  resource;
};

let useRefresh = () => {
  let (version, setVersion) = React.useState(() => 0);

  let refresh = () => setVersion(currentVersion => currentVersion + 1);

  (string_of_int(version), refresh);
};

let componentList = (mapper, items) => items |> List.map(mapper) |> Array.of_list |> ReasonReact.array;

let componentListWithIndex = (mapper, items) => items |> List.mapi(mapper) |> Array.of_list |> ReasonReact.array;

let withDataAttributes = (~data, element) => ReasonReact.cloneElement(element, ~props=Obj.magic(Js.Dict.fromList(data)), [||]);

let withDataCy = (dataCy, element) =>
  switch (dataCy) {
  | Some(dataCy) => withDataAttributes(~data=[("data-cy", dataCy)], element)
  | None => element
  };

let displayResource = (resource, displayLoadedResource) =>
  switch (resource) {
  | Loading => <div> <Translation id="common.loading" /> </div>
  | Loaded(loadedResource) => displayLoadedResource(loadedResource)
  | Failure => <div> <Translation id="common.error" /> </div>
  };

let intOfString = (string: string): option(int) =>
  switch (int_of_string(string)) {
  | int => Some(int)
  | exception (Failure(_)) => None
  };