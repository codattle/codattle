[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

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

  <div> <NavigationBar /> page </div>;
};