open Rationale.RList;
open Rationale.Option;
open Rationale.Option.Infix;
open Rationale.Function;

type translatedText = {
  language: Language.languageName,
  content: string,
};

type translatedTextJs = {
  .
  "language": Language.languageName,
  "content": string,
};

type t = list(translatedText);

let fromJs: array(translatedTextJs) => t = Array.map(x => {language: x##language, content: x##content}) ||> Array.to_list;

let get = (language, i18nText) => i18nText |> find(x => x.language === language) |> fmap(x => x.content);

let getOrBestMatch = (language, i18nText) =>
  i18nText |> get(language) |? (i18nText |> get(Language.defaultLanguage.name)) |? (i18nText |> head |> map(x => x.content));