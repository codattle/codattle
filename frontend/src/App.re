module ReactIntlProvider = {
  [@react.component]
  let make = (~locale, ~messages, ~children) => ReasonReact.element(ReactIntl.IntlProvider.make(~locale, ~messages, children));
};

[@react.component]
let make = () => {
  let (language, setLanguage) = React.useState(() => Language.defaultLanguage);

  <Keycloak.Provider>
    <Language.Provider value=language>
      <ReactIntlProvider locale={language.locale} messages={language.translations}>
        <Notifications.Provider>
          <div>
            <NavigationBar changeLanguage={language => language |> Rationale.Option.iter(language => setLanguage(_ => language))} />
            <AppRouter />
          </div>
        </Notifications.Provider>
      </ReactIntlProvider>
    </Language.Provider>
  </Keycloak.Provider>;
};