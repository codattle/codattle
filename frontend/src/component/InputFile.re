[@react.component]
let make = (~label=?, ~onChange=?, ~dataCy=?) => {
  let getFileFromEvent = (event): option(File.t) => {
    let event = ReactEvent.Form.target(event);
    event##validity##valid ? ListUtils.first(event##files) : None;
  };

  let labelElement = label->Belt.Option.mapWithDefault(<> </>, label => <Translation id=label />);

  <div>
    labelElement
    <input
      type_="file"
      onChange={event => (onChange, getFileFromEvent(event)) |> OptionUtils.ifSome2((onChange, file) => onChange(file))}
    />
  </div>
  |> Utils.withDataCy(dataCy);
};