open Rationale.Option;

module Styles = {
  open Css;

  let noDescription = style([fontStyle(`italic)]);
  let container = style([whiteSpace(`preWrap)]);
};

[@react.component]
let make = (~description: I18nText.t) => {
  let language = Language.useLanguage();

  <ExpansionPanel header="scriptWizard.description">
    <span className=Styles.container>
      {description
       |> I18nText.getOrBestMatch(language.name)
       <$> (desc => ReasonReact.string("\'" ++ desc ++ "\'"))
       |> default(<Translation id="scriptWizard.noDescription" className=Styles.noDescription />)}
    </span>
  </ExpansionPanel>;
};