open Rationale.RList;
open OptionUtils.Infix;

module Styles = {
  open Css;

  let tabs = style([marginLeft(10 |> px), marginRight(10 |> px)]);
  let toolbarButton = style([marginLeft(auto), marginRight((-11) |> px)]);
};

type navigation = {
  name: string,
  href: string,
};

[@react.component]
let make = (~changeLanguage=?) => {
  let url = ReasonReactRouterUtils.useUrlPath();
  let language = Language.useLanguage();
  let languages = [Language.en, Language.pl] |> Selector.Optional.fromListWithSelected(Rationale.Util.identical(language));
  let (keycloak, _) = Keycloak.useKeycloak();

  let navigation = [
    {name: "navigationBar.dashboard", href: "/dashboard"},
    {name: "navigationBar.createGame", href: "/new-game"},
    {name: "navigationBar.games", href: "/games"},
    {name: "navigationBar.profile", href: "/profile"},
  ];

  let authButton =
    keycloak |> Keycloak.authenticated
      ? <Button onClick={() => Keycloak.logout(keycloak)} variant=`Contained> <Translation id="authorization.action.logout" /> </Button>
      : <Button onClick={() => Keycloak.login(keycloak)} variant=`Contained> <Translation id="authorization.action.login" /> </Button>;

  <AppBar position=`Static>
    <Toolbar>
      <Button onClick={() => ReasonReactRouter.push("/")} variant=`Text> {ReasonReact.string("Codattle")} </Button>
      <Tabs
        value={navigation |> containsWith(({href}) => href === url) ? Tabs.selected(url) : Tabs.unselected}
        onChange={(_, href) => ReasonReactRouter.push(href)}
        className=Styles.tabs>
        {navigation |> Utils.componentList(({name, href}) => <Tab value=href label={<Translation id=name />} />)}
      </Tabs>
      <Select
        value=languages
        nameMapper={({name}) => React.string(name |> Language.languageNameToJs)}
        onChange={({selected}) => selected |?> changeLanguage}
        variant=`Negative
      />
      <span className=Styles.toolbarButton> authButton </span>
    </Toolbar>
  </AppBar>;
};