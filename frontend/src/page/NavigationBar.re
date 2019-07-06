[@react.component]
let make = (~changeLocale=?) => {
  <div>
    <Link href="/"> {ReasonReact.string("Home")} </Link>
    <Link href="/dashboard"> {ReasonReact.string("Dashboard")} </Link>
    <Link href="/new-game"> {ReasonReact.string("Create game")} </Link>
    <Link href="/games"> {ReasonReact.string("Games")} </Link>
    <Link href="/profile"> {ReasonReact.string("Profile")} </Link>
    <Button onClick={() => changeLocale |> Utils.ifSome(changeLocale=>changeLocale(Locale.en))}> {ReasonReact.string("EN")} </Button>
    <Button onClick={() => changeLocale |> Utils.ifSome(changeLocale=>changeLocale(Locale.pl))}> {ReasonReact.string("PL")} </Button>
  </div>;
};