[@bs.module] external enTranslations: Js.Dict.t(string) = "./translations/en.yaml";
[@bs.module] external plTranslations: Js.Dict.t(string) = "./translations/pl.yaml";

[@bs.deriving jsConverter]
type languageName = [ | `EN | `PL];

type t = {
  name: languageName,
  locale: string,
  translations: Js.Dict.t(string),
};

let en = {name: `EN, locale: "en", translations: enTranslations};
let pl = {name: `PL, locale: "en", translations: plTranslations};

let defaultLanguage = en;

let context = React.createContext(defaultLanguage);

module Provider = {
  let makeProps = (~value: t, ~children: 'a, ()) => {"value": value, "children": children};
  let make = React.Context.provider(context);
};

let useLanguage = () => React.useContext(context);