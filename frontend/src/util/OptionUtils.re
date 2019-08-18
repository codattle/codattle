let ifSome = (action, item) => Belt.Option.mapWithDefault(item, (), action);

let ifSome2 = (action, (item1, item2)) => item1 |> ifSome(item1 => item2 |> ifSome(item2 => action(item1, item2)));

let executeIfSome = (argument, action) => action |> ifSome(action => action(argument));