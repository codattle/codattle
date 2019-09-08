[@bs.module] external enTranslations: Js.Dict.t(string) = "./translations/en.yaml";
[@bs.module] external plTranslations: Js.Dict.t(string) = "./translations/pl.yaml";

type languageName = [ | `EN | `PL];

type t = {
  name: languageName,
  locale: string,
  translations: Js.Dict.t(string),
};

let en = {name: `EN, locale: "en", translations: enTranslations};
let pl = {name: `PL, locale: "en", translations: plTranslations};

let defaultLanguage = en;

module Context = {
  let context = React.createContext(defaultLanguage);
  let provider = React.Context.provider(context);
};

let useLanguage = () => React.useContext(Context.context);