module Styles = {
  open Css;

  let lstinline = style([fontStyle(`italic), fontWeight(`bold)]);
};

let gameTransaction = (id: string) => <Translation id={"tutorial.howToMakeGame." ++ id} />;

[@react.component]
let make = () =>
  <div>
    <h1> {gameTransaction("header")} </h1>
    <span> {gameTransaction("description")} </span>
    <br />
    <br />
    <span> {gameTransaction("functions.description")} </span>
    <ul>
      <li>
        <span className=Styles.lstinline> {ReasonReact.string("countOfPlayers() : number")} </span>
        <span> {ReasonReact.string("- Zwraca ilość graczy uczestniczących w aktualnej rozgrywce")} </span>
      </li>
      <li>
        <span className=Styles.lstinline> {ReasonReact.string("runPlayerScript(playerIndex: any, gameState: any) : any")} </span>
        <span>
          {ReasonReact.string(
             "- Zwraca obiekt odpowiedzi wygenerowany przez kod gracza o wskazanym indeksie, dodatkowo umożliwia przekazanie mu dowolnego obiektu stanu, który nie musi być identyczny dla każdego gracza. Obiekt \lstinline|gameState|, powinien zostać szczegółowo opisany w sposób zrozumiały dla osób, które będą korzystać z gry",
           )}
        </span>
      </li>
      <li>
        <span className=Styles.lstinline> {ReasonReact.string("end(newWinderIndex: any): void")} </span>
        <span>
          {ReasonReact.string(
             "Kończy rozgrywkę, wybiera gracza o wskazanym indeksie jako zwycięzcę gry. Po wywołaniu tej funkcji, kod gry powinien zostać zakończony.",
           )}
        </span>
      </li>
      <li>
        <span className=Styles.lstinline> {ReasonReact.string("emitFrame(frame: any) : any")} </span>
        <span>
          {ReasonReact.string(
             "- Zapisuje pojedynczą ramkę w formacie tekstowym lub graficznym, która po zakończeniu rozgrywki będzie możliwa do wyświetlania przez uczestników. Jako parametr należy podać obiekt zawierający następującego pola:",
           )}
        </span>
        <ul>
          <li>
            <span className=Styles.lstinline> {ReasonReact.string("mode")} </span>
            <span> {ReasonReact.string("Rodzaj ramki („text” lub „2d”)")} </span>
          </li>
          <li>
            <span className=Styles.lstinline> {ReasonReact.string("output")} </span>
            <span> {ReasonReact.string("Tylko w przypadku ramki typu „text”, pole zawiera tekst do wyświetlenia")} </span>
          </li>
          <li>
            <span className=Styles.lstinline> {ReasonReact.string("width")} </span>
            <span> {ReasonReact.string("Tylko w przypadku ramki typu „2d”, pole zawiera szerokość grafiki")} </span>
          </li>
          <li>
            <span className=Styles.lstinline> {ReasonReact.string("height")} </span>
            <span> {ReasonReact.string("Tylko w przypadku rami typu „2d”, pole zawiera wysokość grafiki")} </span>
          </li>
          <li>
            <span className=Styles.lstinline> {ReasonReact.string("objects")} </span>
            <span> {ReasonReact.string("Tylko w przypadku ramki typu „2d”")} </span>
            <ul>
              <li>
                <span className=Styles.lstinline> {ReasonReact.string("x")} </span>
                <span> {ReasonReact.string("Położenie „x” obiektu")} </span>
              </li>
              <li>
                <span className=Styles.lstinline> {ReasonReact.string("y")} </span>
                <span> {ReasonReact.string("Położenie „y” obiektu")} </span>
              </li>
              <li>
                <span className=Styles.lstinline> {ReasonReact.string("width")} </span>
                <span> {ReasonReact.string("Szerokość obrazka")} </span>
              </li>
              <li>
                <span className=Styles.lstinline> {ReasonReact.string("height")} </span>
                <span> {ReasonReact.string("Wysokość obrazka")} </span>
              </li>
              <li>
                <span className=Styles.lstinline> {ReasonReact.string("sprite")} </span>
                <span> {ReasonReact.string("Nazwa obiektu do wyświetlenia")} </span>
              </li>
            </ul>
          </li>
        </ul>
      </li>
    </ul>
  </div>;