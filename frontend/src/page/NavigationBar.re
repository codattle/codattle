open Rationale.Option;

module Styles = {
  open Css;

  let tabs = style([marginLeft(10 |> px), marginRight(10 |> px)]);
};

type state = {tab: option(string)};

type action = {tab: option(string)};

type navigation = {
  name: string,
  href: string,
};

[@react.component]
let make = (~changeLanguage=?) => {
  let (state, send) =
    ReactUpdate.useReducer({tab: None}, ({tab}, _) =>
      ReactUpdate.UpdateWithSideEffects(
        {tab: tab},
        _ => {
          tab |> default("/") |> ReasonReactRouter.push;
          None;
        },
      )
    );
  let language = Language.useLanguage();

  let navigation =
    [
      {name: "navigationBar.dashboard", href: "/dashboard"},
      {name: "navigationBar.createGame", href: "/new-game"},
      {name: "navigationBar.games", href: "/games"},
      {name: "navigationBar.profile", href: "/profile"},
    ]
    |> List.map(({name, href}) => {name: language.translations->Js.Dict.get(name) |> default(name), href});

  <AppBar position=`Static>
    <Toolbar>
      <Button onClick={() => send({tab: None})} variant=`Text> {ReasonReact.string("Codattle")} </Button>
      <Tabs
        value={state.tab |> map(href => Tabs.selected(href)) |> default(Tabs.unselected)}
        onChange={(_, href) => send({tab: Some(href)})}
        className=Styles.tabs>
        {navigation |> Utils.componentList(({name, href}) => <Tab value=href label=name />)}
      </Tabs>
      <Select
        value=language
        items=[(Language.en, ReasonReact.string("EN")), (Language.pl, ReasonReact.string("PL"))]
        onChange={language => changeLanguage |> OptionUtils.execIfSome(language)}
      />
    </Toolbar>
  </AppBar>;
};