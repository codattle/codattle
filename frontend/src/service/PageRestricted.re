[@react.component]
let make = () => {
  let (keycloak, _) = Keycloak.useKeycloak();

  React.useEffect0(() => {
    Keycloak.login(keycloak);
    None;
  });

  <> </>;
};