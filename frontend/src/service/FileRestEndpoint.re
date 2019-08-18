let sendFile = (file: File.t) =>
  Fetch.fetchWithInit(
    "/api/v1/file",
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=Fetch.BodyInit.makeWithFormData(FormData.createFromDict(Js.Dict.fromList([("file", file)]))),
      (),
    ),
  )
  |> Repromise.Rejectable.fromJsPromise
  |> Repromise.Rejectable.catch(_error => Repromise.Rejectable.rejected("Error occurred while HTTP request"))
  |> Repromise.Rejectable.andThen(response =>
       if (Fetch.Response.ok(response)) {
         Fetch.Response.json(response)
         |> Repromise.Rejectable.fromJsPromise
         |> Repromise.Rejectable.catch(_error => Repromise.Rejectable.rejected("Cannot parse HTTP response as JSON"));
       } else {
         Repromise.Rejectable.rejected("Response hasn't ok status");
       }
     )
  |> Repromise.Rejectable.andThen(json =>
       Json.Decode.(
         switch (json |> field("id", string)) {
         | id => Repromise.Rejectable.resolved(id)
         | exception (DecodeError(_)) => Repromise.Rejectable.rejected("JSON from HTTP response has invalid format")
         }
       )
     );