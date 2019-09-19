module ReactIntlProvider = {
  [@react.component]
  let make = (~locale, ~messages, ~children) => ReasonReact.element(ReactIntl.IntlProvider.make(~locale, ~messages, children));
};

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let (language, setLanguage) = React.useState(() => Language.defaultLanguage);

  let page =
    switch (url.path) {
    | [] => <HomePage />
    | ["dashboard"] => <Dashboard />
    | ["profile"] => <UserProfile />
    | ["new-game"] => <GameWizard />
    | ["games"] => <GameList />
    | ["games", gameId] => <GameDetails gameId />
    | ["games", gameId, "new-match"] => <MatchWizard gameId />
    | ["games", gameId, "matches"] => <MatchList gameId />
    | ["games", gameId, "my-scripts"] => <MyScripts gameId />
    | ["games", gameId, "script-testing"] => <ScriptTesting gameId />
    | ["games", "matches", matchId] => <MatchDetails matchId />
    | ["games", "matches", matchId, "new-script"] => <ScriptWizard matchId />
    | ["games", "scripts", scriptId] => <ScriptDetails scriptId />
    | _ => <NotFoundPage />
    };

  <Language.Provider value=language>
    <ReactIntlProvider locale={language.locale} messages={language.translations}>
      <Notifications.Provider>
        <div>
          <NavigationBar changeLanguage={language => language |> Rationale.Option.iter(language => setLanguage(_ => language))} />
          page
        </div>
      </Notifications.Provider>
    </ReactIntlProvider>
  </Language.Provider>;
};