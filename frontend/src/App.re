[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let (locale, setLocale) = React.useState(() => Locale.en);

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
    | ["games", "matches", matchId] => <MatchDetails matchId />
    | ["games", "matches", matchId, "new-script"] => <ScriptWizard matchId />
    | _ => <NotFoundPage />
    };

  ReasonReact.element(
    ReactIntl.IntlProvider.make(
      ~locale="en",
      ~messages=locale.translations,
      <div> <NavigationBar changeLocale={locale => setLocale(_ => locale)} /> page </div>,
    ),
  );
};