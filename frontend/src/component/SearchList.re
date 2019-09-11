type filters = {name: string};

type display = {
  id: string,
  name: string,
};

module Styles = {
  open Css;
  open Style.Color;

  let itemsContainer = style([width(200 |> px)]);

  let item =
    style([
      display(`flex),
      alignItems(`center),
      height(30 |> px),
      marginTop(10 |> px),
      marginBottom(10 |> px),
      padding(5 |> px),
      backgroundColor(lightgray),
      cursor(`pointer),
      hover([backgroundColor(lightgray |> darken(0.2))]),
    ]);

  let itemTitle = style([fontWeight(`bold), overflow(`hidden), whiteSpace(`nowrap), textOverflow(`ellipsis)]);
};

[@react.component]
let make =
    (
      ~search: filters => GraphqlService.query('result),
      ~queryMapper: 'result => list('item),
      ~displayMapper: 'item => display,
      ~onItemClick: option('item => unit)=?,
    ) => {
  let (filters, setFilters) = React.useState(() => ({name: ""}: filters));
  let items = Utils.useResourceWithDebounce(search(filters), [|filters|], 500, queryMapper);

  <div>
    <TextField label="searchList.name" onChange={name => setFilters(_ => {name: name})} />
    <div className=Styles.itemsContainer>
      {switch (items) {
       | NotLoaded => <> </>
       | Loading => <Translation id="searchList.searching" />
       | Loaded(items) =>
         items
         |> Utils.componentList(item => {
              let {id, name} = displayMapper(item);
              <div key=id className=Styles.item onClick={_ => onItemClick |> OptionUtils.execIfSome(item)}>
                <span className=Styles.itemTitle> {ReasonReact.string(name)} </span>
              </div>;
            })
       | Failure => <Translation id="searchList.error" />
       }}
    </div>
  </div>;
};