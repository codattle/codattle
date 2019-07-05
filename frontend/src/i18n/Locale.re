[@bs.module] external enTranslations: Js.Dict.t(string) = "./translations/en.yaml";
[@bs.module] external plTranslations: Js.Dict.t(string) = "./translations/pl.yaml";

type locale = {
  name: string,
  translations: Js.Dict.t(string),
};

let en = {name: "en", translations: enTranslations};
let pl = {name: "pl", translations: plTranslations};