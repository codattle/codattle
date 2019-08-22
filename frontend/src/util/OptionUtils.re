open Rationale.Option;

let ifSome2 = (action, (item1, item2)) => item1 |> iter(item1 => item2 |> iter(item2 => action(item1, item2)));

let execIfSome = (argument, action) => action |> iter(action => action(argument));