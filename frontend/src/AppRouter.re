[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let (keycloak, initialized) = Keycloak.useKeycloak();

  let requireAuthorization = page => keycloak |> Keycloak.authenticated ? page : <PageRestricted />;

  if (initialized) {
    switch (url.path) {
    | [] => <HomePage />
    | ["dashboard"] => <Dashboard /> |> requireAuthorization
    | ["profile"] => <UserProfile /> |> requireAuthorization
    | ["new-game"] => <GameWizard /> |> requireAuthorization
    | ["games"] => <GameList /> |> requireAuthorization
    | ["games", gameId] => <GameDetails gameId /> |> requireAuthorization
    | ["games", gameId, "new-match"] => <MatchWizard gameId /> |> requireAuthorization
    | ["games", gameId, "new-tournament"] => <TournamentWizard gameId /> |> requireAuthorization
    | ["games", gameId, "matches"] => <MatchList gameId /> |> requireAuthorization
    | ["games", gameId, "my-scripts"] => <MyScripts gameId /> |> requireAuthorization
    | ["games", gameId, "script-testing"] => <ScriptTesting gameId /> |> requireAuthorization
    | ["games", gameId, "tournaments"] => <TournamentList gameId /> |> requireAuthorization
    | ["games", "matches", matchId] => <MatchDetails matchId /> |> requireAuthorization
    | ["games", "matches", matchId, "new-script"] => <ScriptWizard matchId /> |> requireAuthorization
    | ["games", "scripts", scriptId] => <ScriptDetails scriptId /> |> requireAuthorization
    | ["games", "tournaments", tournamentId] => <TournamentDetails tournamentId /> |> requireAuthorization
    | ["games", "tournaments", tournamentId, "join"] => <TournamentJoin tournamentId /> |> requireAuthorization
    | _ => <NotFoundPage />
    };
  } else {
    <> </>;
  };
};