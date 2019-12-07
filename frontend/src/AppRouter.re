[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let (keycloak, _) = Keycloak.useKeycloak();

  keycloak |> Keycloak.authenticated
    ? switch (url.path) {
      | [] => <HomePage />
      | ["dashboard"] => <Dashboard />
      | ["profile"] => <UserProfile />
      | ["new-game"] => <GameWizard />
      | ["games"] => <GameList />
      | ["games", gameId] => <GameDetails gameId />
      | ["games", gameId, "new-match"] => <MatchWizard gameId />
      | ["games", gameId, "new-tournament"] => <TournamentWizard gameId />
      | ["games", gameId, "matches"] => <MatchList gameId />
      | ["games", gameId, "my-scripts"] => <MyScripts gameId />
      | ["games", gameId, "script-testing"] => <ScriptTesting gameId />
      | ["games", gameId, "tournaments"] => <TournamentList gameId />
      | ["games", "matches", matchId] => <MatchDetails matchId />
      | ["games", "matches", matchId, "new-script"] => <ScriptWizard matchId />
      | ["games", "scripts", scriptId] => <ScriptDetails scriptId />
      | ["games", "tournaments", tournamentId] => <TournamentDetails tournamentId />
      | ["games", "tournaments", tournamentId, "join"] => <TournamentJoin tournamentId />
      | _ => <NotFoundPage />
      }
    : <PageRestricted />;
};