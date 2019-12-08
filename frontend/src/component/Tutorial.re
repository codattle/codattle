module Styles = {
  open Css;

  let container = style([padding(30 |> px), maxWidth(1200 |> px), margin2(~v=0 |> px, ~h=`auto), textAlign(`justify)]);
  let lstinline = style([fontStyle(`italic), fontWeight(`bold)]);
};

let gameTranslationKey = (id: string) => "tutorial.howToMakeGame." ++ id;
let playTranslationKey = (id: string) => "tutorial.howToPlayGame." ++ id;

let pointOfList = (getTranslationKey, ids: list(int)) => {
  let getKey = value =>
    getTranslationKey(
      (ids |> List.map(id => string_of_int(id)) |> List.fold_left((text, id) => text ++ "fn" ++ id ++ ".", "functions.")) ++ value,
    );

  <span>
    <Translation className=Styles.lstinline id={getKey("key")} />
    <span> {ReasonReact.string(" - ")} </span>
    <Translation id={getKey("value")} />
  </span>;
};

let headerSection = getTranslationKey => {
  <div>
    <h1> <Translation id={getTranslationKey("header")} /> </h1>
    <Translation id={getTranslationKey("description")} />
    <br />
    <br />
    <Translation id={getTranslationKey("functions.description")} />
  </div>;
};

let gamePointOfList = (ids: list(int)) => pointOfList(gameTranslationKey, ids);
let playPointOfList = (ids: list(int)) => pointOfList(playTranslationKey, ids);

let howToMakeGame =
  <div>
    {headerSection(gameTranslationKey)}
    <ul>
      <li> {gamePointOfList([1])} </li>
      <li> {gamePointOfList([2])} </li>
      <li> {gamePointOfList([3])} </li>
      <li>
        {gamePointOfList([4])}
        <ul>
          <li> {gamePointOfList([4, 1])} </li>
          <li> {gamePointOfList([4, 2])} </li>
          <li> {gamePointOfList([4, 3])} </li>
          <li> {gamePointOfList([4, 4])} </li>
          <li>
            {gamePointOfList([4, 5])}
            <ul>
              <li> {gamePointOfList([4, 5, 1])} </li>
              <li> {gamePointOfList([4, 5, 2])} </li>
              <li> {gamePointOfList([4, 5, 3])} </li>
              <li> {gamePointOfList([4, 5, 4])} </li>
              <li> {gamePointOfList([4, 5, 5])} </li>
            </ul>
          </li>
        </ul>
      </li>
    </ul>
  </div>;

let howToPlayGame =
  <div>
    {headerSection(playTranslationKey)}
    <ul>
      <li> {playPointOfList([1])} </li>
      <li> {playPointOfList([2])} </li>
      <li> {playPointOfList([3])} </li>
      <li> {playPointOfList([4])} </li>
    </ul>
  </div>;

[@react.component]
let make = () => <div className=Styles.container> howToMakeGame howToPlayGame </div>;