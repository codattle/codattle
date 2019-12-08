type rating = {
  value: int,
  description: option(string),
};

module Styles = {
  open Css;

  let section = style([marginTop(10 |> px)]);
};

[@react.component]
let make = (~onSend: option(rating => unit)=?, ~editRating: bool=false) => {
  let (ratingValue, setRatingValue) = React.useState(() => 3);
  let (description, setDescription) = React.useState(() => "");

  <div>
    <Rating initialValue=ratingValue onChange={value => setRatingValue(_ => value)} />
    <TextArea onChange={value => setDescription(_ => value)} />
    <div className=Styles.section>
      <Button
        label={editRating ? "ratingForm.edit" : "ratingForm.send"}
        onClick={() => onSend |> OptionUtils.execIfSome({value: ratingValue, description: description |> StringUtils.toNonBlank})}
      />
    </div>
  </div>;
};