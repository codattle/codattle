let ifSome2 = (action: ('a, 'b) => unit, (item1: option('a), item2: option('b))): unit =>
  switch (item1, item2) {
  | (Some(item1), Some(item2)) => action(item1, item2)
  | _ => ()
  };

let execIfSome = (argument: 'a, action: option('a => unit)) =>
  switch (action) {
  | Some(action) => action(argument)
  | _ => ()
  };

let default = (defaultProvider: unit => 'a, option: option('a)): 'a =>
  switch (option) {
  | Some(value) => value
  | None => defaultProvider()
  };

module Infix = {
  let (|?>) = (argument: 'a, action: option('a => unit)): unit => execIfSome(argument, action);
  let (||?) = (option: option('a), default: 'a): 'a => option |> Rationale.Option.default(default);
};