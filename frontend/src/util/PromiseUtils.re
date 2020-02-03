open Rationale.Function.Infix;

type t('a) = Promise.t(Belt.Result.t('a, unit));

[@bs.scope "Promise"] [@bs.val] external jsAll: 'a => 'b = "all";

let resolved = (value: 'a): t('a) => value |> Rationale.Result.return |> Promise.resolved;

let fromJsPromise = (promise: Js.Promise.t('a)): t('a) =>
  promise
  ->Promise.Js.fromBsPromise
  ->Promise.Js.toResult
  ->Promise.mapError(ignore)

let toJsPromise = (promise: t('a)): Js.Promise.t('a) =>
  promise
  ->Promise.Js.relax
  ->Promise.Js.flatMap(
       Rationale.Option.ofResult
       ||> Rationale.Option.map(Promise.Js.resolved)
       ||> OptionUtils.default(() => Promise.Js.rejected()),
     )
  ->Promise.Js.toBsPromise;

let fromRejectable = (promise: Promise.Js.t('a, _)): t('a) => promise |> Promise.Js.toBsPromise |> fromJsPromise;

let toRejectable = (promise: t('a)): Promise.Js.t('a, _) => promise |> toJsPromise |> Promise.Js.fromBsPromise;

let flatMapOk =
    (map: 'a => Belt.Result.t('b, unit), promise: Promise.t(Belt.Result.t('a, unit))): Promise.t(Belt.Result.t('b, unit)) =>
  promise->Promise.mapOk(map)->Promise.map(Rationale.Result.join);

let all = (promises: list(t('a))): t(list('a)) => promises |> List.map(toRejectable) |> Promise.Js.all |> fromRejectable;

let all2 = (promise1: t('a), promise2: t('b)): t(('a, 'b)) =>
  jsAll((promise1 |> toJsPromise, promise2 |> toJsPromise)) |> fromJsPromise;