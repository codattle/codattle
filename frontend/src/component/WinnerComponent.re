[@react.component]
let make = (~winner: option(int)) => {
  Belt.Option.mapWithDefault(winner, <> </>, winner =>
    <span>
      {ReasonReact.string(" ")}
      <Translation id="matchList.winner" />
      {ReasonReact.string(": \"")}
      <Translation id="matchList.player" />
      {ReasonReact.string(" " ++ string_of_int(winner + 1) ++ "\"")}
    </span>
  );
};