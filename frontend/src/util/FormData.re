type t = Fetch.formData;

[@bs.new] external create: unit => t = "FormData";

[@bs.send.pipe: t] external append: (string, 'a) => unit = "append";

let createFromDict = (dict: Js.Dict.t('a)): t => {
  let formData = create();
  dict |> Js.Dict.entries |> Js.Array.forEach(((key, value)) => formData |> append(key, value));
  formData;
};