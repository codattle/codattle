let sendFile = (file: File.t): PromiseUtils.t(string) =>
  Fetch.fetchWithInit(
    "/api/v1/file",
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=Fetch.BodyInit.makeWithFormData(FormData.createFromDict(Js.Dict.fromList([("file", file)]))),
      (),
    ),
  )
  |> FetchUtils.parseJson
  |> PromiseUtils.flatMapOk(json =>
       Json.Decode.(
         switch (json |> field("id", string)) {
         | id => Belt.Result.Ok(id)
         | exception (DecodeError(_)) => Belt.Result.Error()
         }
       )
     );