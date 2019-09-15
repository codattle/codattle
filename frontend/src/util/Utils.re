type resource('a) =
  | NotLoaded
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

let useEditableResource = (query, shouldRefresh, mapper) => {
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
  let (resource, setResource) = React.useState(() => NotLoaded);
  let (loadResourceDebouncer, _) =
    React.useState(() => Debouncer.makeCancelable(~wait, ((query, setResource, mapper)) => loadResource(query, setResource, mapper)));

  useEffectWithInit(
    () => loadResourceDebouncer.invoke((query, setResource, mapper)),
    () => loadResourceDebouncer.schedule((query, setResource, mapper)),
    // TODO: this cancels only scheduled invocation of request, sent request won't be cancelled,
    // request should be unsubscribed using AbortController
    // https://developer.mozilla.org/en-US/docs/Web/API/AbortController
    Some(loadResourceDebouncer.cancel),
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
  | NotLoaded => <div />
  | Loading => <div> <Translation id="common.loading" /> </div>
  | Loaded(loadedResource) => displayLoadedResource(loadedResource)
  | Failure => <div> <Translation id="common.error" /> </div>
  };