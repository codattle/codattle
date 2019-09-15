open Rationale.Option;

module Styles = {
  open Css;

  let noDescription = style([fontStyle(`italic)]);
};

[@react.component]
let make = (~description: I18nText.t) => {
  let language = Language.useLanguage();

  <ExpansionPanel header="scriptWizard.description">
    {description
     |> I18nText.getOrBestMatch(language.name)
     <$> ReasonReact.string
     |> default(<Translation id="scriptWizard.noDescription" className=Styles.noDescription />)}
  </ExpansionPanel>;
};