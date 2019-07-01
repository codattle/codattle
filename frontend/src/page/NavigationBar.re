[@react.component]
let make = () => {
  <div>
    <Link href="/"> {ReasonReact.string("Home")} </Link>
    <Link href="/dashboard"> {ReasonReact.string("Dashboard")} </Link>
    <Link href="/new-game"> {ReasonReact.string("Create game")} </Link>
    <Link href="/games"> {ReasonReact.string("Games")} </Link>
    <Link href="/profile"> {ReasonReact.string("Profile")} </Link>
  </div>;
};