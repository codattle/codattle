open OptionUtils;

[@react.component]
let make = (~changeLanguage=?) => {
  <div>
    <Link href="/"> {ReasonReact.string("Home")} </Link>
    <Link href="/dashboard"> {ReasonReact.string("Dashboard")} </Link>
    <Link href="/new-game"> {ReasonReact.string("Create game")} </Link>
    <Link href="/games"> {ReasonReact.string("Games")} </Link>
    <Link href="/profile"> {ReasonReact.string("Profile")} </Link>
    <Button onClick={() => changeLanguage |> execIfSome(Language.en)}> {ReasonReact.string("EN")} </Button>
    <Button onClick={() => changeLanguage |> execIfSome(Language.pl)}> {ReasonReact.string("PL")} </Button>
  </div>;
};