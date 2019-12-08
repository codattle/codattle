module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(800 |> px)]);
  let lstinline = style([fontStyle(`italic), fontWeight(`bold)]);
};

let gameTransactionKey = (id: string) => "tutorial.howToMakeGame." ++ id;

let pointOfList = (ids: list(string)) => {
  let key = (value: string) =>
    gameTransactionKey((ids |> List.fold_left((text, id) => text ++ "fn" ++ id ++ ".", "functions.")) ++ value);
  <span>
    <Translation className=Styles.lstinline id={key("key")} />
    <span> {ReasonReact.string(" - ")} </span>
    <Translation id={key("value")} />
  </span>;
};

[@react.component]
let make = () =>
  <div className=Styles.container>
    <h1> <Translation id={gameTransactionKey("header")} /> </h1>
    <Translation id={gameTransactionKey("description")} />
    <br />
    <br />
    <Translation id={gameTransactionKey("functions.description")} />
    <ul>
      <li> {pointOfList(["1"])} </li>
      <li> {pointOfList(["2"])} </li>
      <li> {pointOfList(["3"])} </li>
      <li>
        {pointOfList(["4"])}
        <ul>
          <li> {pointOfList(["4", "1"])} </li>
          <li> {pointOfList(["4", "2"])} </li>
          <li> {pointOfList(["4", "3"])} </li>
          <li> {pointOfList(["4", "4"])} </li>
          <li>
            {pointOfList(["4", "5"])}
            <ul>
              <li> {pointOfList(["4", "5", "1"])} </li>
              <li> {pointOfList(["4", "5", "2"])} </li>
              <li> {pointOfList(["4", "5", "3"])} </li>
              <li> {pointOfList(["4", "5", "4"])} </li>
              <li> {pointOfList(["4", "5", "5"])} </li>
            </ul>
          </li>
        </ul>
      </li>
    </ul>
  </div>;