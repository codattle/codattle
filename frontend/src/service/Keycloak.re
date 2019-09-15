type keycloak;
type init;
type initParam;
type success;
type error;

[@bs.deriving abstract]
type config = {
  url: string,
  realm: string,
  clientId: string,
};

[@bs.deriving abstract]
type initConfig = {onLoad: string};

[@bs.module] external keycloak: config => keycloak = "keycloak-js";
[@bs.obj] external makeInitParam: (~onLoad: string, unit) => initParam = "";

let initConfig = initConfig(~onLoad="login-required");
let keycloakConfig = config(~url="http://localhost:8081/auth", ~realm="Keycloak", ~clientId="keycloak-client");
let keycloak = keycloakConfig |> keycloak;

module Authorization = {
  [@bs.module "react-keycloak"] [@react.component]
  external make: (~keycloak: keycloak, ~initConfig: initConfig, ~children: 'a) => React.element = "KeycloakProvider";
};

module Provider = {
  [@react.component]
  let make = (~children) => <Authorization keycloak initConfig> children </Authorization>;
};