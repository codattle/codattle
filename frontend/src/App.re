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
    | ["games", "matches", matchId] => <MatchDetails matchId />
    | ["games", "matches", matchId, "new-script"] => <ScriptWizard matchId />
    | _ => <NotFoundPage />
    };

  React.createElement(
    Language.Context.provider,
    {
      "value": language,
      "children":
        ReasonReact.element(
          {
            ReactIntl.IntlProvider.make(
              ~locale=language.locale,
              ~messages=language.translations,
              <div> <NavigationBar changeLanguage={language => setLanguage(_ => language)} /> page </div>,
            );
          },
        ),
    },
  );
};