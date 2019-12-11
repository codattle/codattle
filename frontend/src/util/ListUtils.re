let empty = list => List.length(list) == 0;

let notEmpty = (list: list('a)): bool => !empty(list);

let first = list => empty(list) ? None : Some(List.hd(list));

let split = (pred: 'a => bool, list: list('a)): option((list('a), 'a, list('a))) =>
  switch (list |> Rationale.RList.splitWhen(pred)) {
  | (before, [selected, ...after]) => Some((before, selected, after))
  | _ => None
  };

let splitLast = (list: list('a)): (list('a), option('a)) => (list |> Rationale.RList.dropLast(1), list |> Rationale.RList.last);