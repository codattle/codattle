let empty = list => List.length(list) == 0;

let first = list => empty(list) ? None : Some(List.hd(list));

let split = (pred: 'a => bool, list: list('a)): option((list('a), 'a, list('a))) =>
  switch (list |> Rationale.RList.splitWhen(pred)) {
  | (before, [selected, ...after]) => Some((before, selected, after))
  | _ => None
  };