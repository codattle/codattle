open Rationale.RList;
open OptionUtils.Infix;

module Styles = {
  open Css;
  open Style.Color;

  let tabs = style([marginLeft(10 |> px), marginRight(10 |> px)]);
  let toolbarButton = style([marginLeft(20 |> px), marginRight((-11) |> px)]);
  let tabsIndicator = style([backgroundColor(primaryColor) |> important, height(4 |> px) |> important]);
  let tabRoot = style([minWidth(0 |> px) |> important, paddingLeft(30 |> px) |> important, paddingRight(30 |> px) |> important]);
  let tabWrapper = style([fontSize(13 |> px), fontWeight(bold)]);
  let languageSelector = style([marginLeft(auto)]);
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
      <Button onClick={() => ReasonReactRouter.push("/")} variant=`Text disableRipple=true> {ReasonReact.string("Codattle")} </Button>
      <Tabs
        value={navigation |> containsWith(({href}) => href === url) ? Tabs.selected(url) : Tabs.unselected}
        onChange={(_, href) => ReasonReactRouter.push(href)}
        className=Styles.tabs
        classes={Js.Dict.fromList([("indicator", Styles.tabsIndicator)])}>
        {navigation
         |> Utils.componentList(({name, href}) =>
              <Tab
                value=href
                label={<Translation id=name />}
                classes={Js.Dict.fromList([("root", Styles.tabRoot), ("wrapper", Styles.tabWrapper)])}
                disableRipple=true
              />
            )}
      </Tabs>
      <div className=Styles.languageSelector>
        <Select
          value=languages
          nameMapper={({name}) => React.string(name |> Language.languageNameToJs)}
          onChange={({selected}) => selected |?> changeLanguage}
          variant=`Negative
        />
      </div>
      <div className=Styles.toolbarButton> authButton </div>
    </Toolbar>
  </AppBar>;
};