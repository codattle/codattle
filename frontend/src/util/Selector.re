open Rationale.Option.Infix;
open OptionUtils.Infix;

type selector('a, 'b) = {
  before: list('a),
  selected: 'b,
  after: list('a),
};
type t('a, 'b) = selector('a, 'b);

module Required = {
  type t('a) = selector('a, 'a);

  let single = (item: 'a): t('a) => {before: [], selected: item, after: []};
  let fromListWithSelected = (pred: 'a => bool, list: list('a)): option(t('a)) => {
    switch (list |> ListUtils.split(pred)) {
    | Some((before, selected, after)) => Some({before, selected, after})
    | None => None
    };
  };
  let all = ({before, selected, after}: t('a)): list('a) => before @ [selected] @ after;
  let select = (pred: 'a => bool, list: t('a)): option(t('a)) => list |> all |> fromListWithSelected(pred);
  let map = (map: 'a => 'b, {before, selected, after}: t('a)): t('b) => {
    before: List.map(map, before),
    selected: map(selected),
    after: List.map(map, after),
  };
};

module Optional = {
  type t('a) = selector('a, option('a));

  let fromList = (list: list('a)): t('a) => {before: list, selected: None, after: []};
  let fromListWithFirstSelected = (list: list('a)): t('a) =>
    switch (list) {
    | [x, ...xs] => {before: [], selected: Some(x), after: xs}
    | _ => {before: [], selected: None, after: []}
    };
  let fromListWithSelected = (pred: 'a => bool, list: list('a)): t('a) => {
    switch (list |> ListUtils.split(pred)) {
    | Some((before, selected, after)) => {before, selected: Some(selected), after}
    | None => fromList(list)
    };
  };
  let all = ({before, selected, after}: t('a)): list('a) => before @ (selected |> Rationale.Option.toList) @ after;
  let select = (pred: 'a => bool, list: t('a)): t('a) => list |> all |> fromListWithSelected(pred);
  let unselect = (list: t('a)): t('a) => list |> all |> fromList;
  let map = (map: 'a => 'b, {before, selected, after}: t('a)): t('b) => {
    before: List.map(map, before),
    selected: selected <$> map,
    after: List.map(map, after),
  };
  let mapi = (map: (int, 'a) => 'b, {before, selected, after}: t('a)): t('b) => {
    let before = before |> List.mapi(map);
    let beforeOffset = before |> List.length;
    let selected = selected <$> map(beforeOffset);
    let offset = beforeOffset + (Rationale.Option.isSome(selected) ? 1 : 0);
    let after = after |> List.mapi((index, item) => map(index + offset, item));
    {before, selected, after};
  };
  let mapSelected = (map: 'a => 'a, {before, selected, after}: t('a)): t('a) => {before, selected: selected <$> map, after};
  let mapFirst = (pred: 'a => bool, map: 'a => 'a, {before, selected, after}: t('a)): t('a) => {
    before
    |> ListUtils.split(pred)
    <$> (((xs, x, ys)) => xs @ [map(x)] @ ys)
    <$> (before => {before, selected, after})
    |> OptionUtils.or_(() =>
         selected |> Rationale.Option.filter(pred) <$> map <$> (selected => {before, selected: Some(selected), after})
       )
    |> OptionUtils.or_(() =>
         after |> ListUtils.split(pred) <$> (((xs, x, ys)) => xs @ [map(x)] @ ys) <$> (after => {before, selected, after})
       )
    ||? {before, selected, after};
  };
  let length = (list: t('a)): int => list |> all |> List.length;
};

let toOptional = ({before, selected, after}: Required.t('a)): Optional.t('a) => {before, selected: Some(selected), after};
let toRequired = ({before, selected, after}: Optional.t('a)): option(Required.t('a)) =>
  selected <$> (selected => {before, selected, after});
let addFirst = (item: 'a, list: t('a, 'b)): t('a, 'b) => {...list, before: [item, ...list.before]};
let addLast = (item: 'a, list: t('a, 'b)): t('a, 'b) => {...list, after: list.after @ [item]};