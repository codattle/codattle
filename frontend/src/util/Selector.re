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
  let fromListWithFirstSelected = (list: list('a)): option(t('a)) =>
    switch (list) {
    | [x, ...xs] => Some({before: [], selected: x, after: xs})
    | _ => None
    };
  let fromListWithSelected = (pred: 'a => bool, list: list('a)): option(t('a)) => {
    switch (list |> ListUtils.split(pred)) {
    | Some((before, selected, after)) => Some({before, selected, after})
    | None => None
    };
  };
  let toList = ({before, selected, after}: t('a)): list('a) => before @ [selected] @ after;
  let select = (pred: 'a => bool, selector: t('a)): option(t('a)) => selector |> toList |> fromListWithSelected(pred);
  let map = (map: 'a => 'b, {before, selected, after}: t('a)): t('b) => {
    before: List.map(map, before),
    selected: map(selected),
    after: List.map(map, after),
  };
  let next = ({before, selected, after} as selector: t('a)): t('a) =>
    switch (after) {
    | [x, ...xs] => {before: before @ [selected], selected: x, after: xs}
    | _ => selector
    };
  let previous = ({before, selected, after} as selector: t('a)): t('a) => {
    switch (ListUtils.splitLast(before)) {
    | (xs, Some(x)) => {before: xs, selected: x, after: [selected] @ after}
    | _ => selector
    };
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
  let toList = ({before, selected, after}: t('a)): list('a) => before @ (selected |> Rationale.Option.toList) @ after;
  let select = (pred: 'a => bool, selector: t('a)): t('a) => selector |> toList |> fromListWithSelected(pred);
  let unselect = (selector: t('a)): t('a) => selector |> toList |> fromList;
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
  let length = (selector: t('a)): int => selector |> toList |> List.length;
  let next = ({before, selected, after} as selector: t('a)): t('a) =>
    switch (after) {
    | [x, ...xs] => {before: before @ Rationale.Option.toList(selected), selected: Some(x), after: xs}
    | _ => selector
    };
  let previous = ({before, selected, after} as selector: t('a)): t('a) => {
    switch (ListUtils.splitLast(before)) {
    | (xs, Some(x)) => {before: xs, selected: Some(x), after: Rationale.Option.toList(selected) @ after}
    | _ => selector
    };
  };
  let makeEmpty = (): t('a) => [] |> fromList;
};

let toOptional = ({before, selected, after}: Required.t('a)): Optional.t('a) => {before, selected: Some(selected), after};
let toRequired = ({before, selected, after}: Optional.t('a)): option(Required.t('a)) =>
  selected <$> (selected => {before, selected, after});
let addFirst = (item: 'a, selector: t('a, 'b)): t('a, 'b) => {...selector, before: [item, ...selector.before]};
let addLast = (item: 'a, selector: t('a, 'b)): t('a, 'b) => {...selector, after: selector.after @ [item]};
let selected = (selector: t(_, 'a)): 'a => selector.selected;
let canNext = ({after}: t(_, _)): bool => ListUtils.notEmpty(after);
let canPrevious = ({before}: t(_, _)): bool => ListUtils.notEmpty(before);