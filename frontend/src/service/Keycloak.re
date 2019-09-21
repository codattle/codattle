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

[@bs.deriving {abstract: light}]
type keycloakT = {authenticated: bool};

[@bs.deriving abstract]
type initConfig = {onLoad: string};

[@bs.module] external keycloak: config => keycloak = "keycloak-js";
[@bs.obj] external makeInitParam: (~onLoad: string, unit) => initParam = "";
[@bs.send] external login: keycloakT => unit = "login";
[@bs.send] external logout: keycloakT => unit = "logout";

let initConfig = initConfig(~onLoad="check-sso");
let keycloakConfig = config(~url="http://localhost:8090/auth", ~realm="Keycloak", ~clientId="keycloak-client");
let keycloak = keycloakConfig |> keycloak;

module Authorization = {
  [@bs.module "react-keycloak"] [@react.component]
  external make: (~keycloak: keycloak, ~initConfig: initConfig, ~children: 'a) => React.element = "KeycloakProvider";
};

module Provider = {
  [@react.component]
  let make = (~children) => <Authorization keycloak initConfig> children </Authorization>;
};

[@bs.module "react-keycloak"] [@bs.val] external useKeycloak: unit => (keycloakT, bool) = "useKeycloak";
let useKeycloak = useKeycloak;