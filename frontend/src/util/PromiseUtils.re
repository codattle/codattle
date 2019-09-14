open Rationale.Function.Infix;

type t('a) = Repromise.t(Belt.Result.t('a, unit));

[@bs.scope "Promise"] [@bs.val] external jsAll: 'a => 'b = "all";

let resolved = (value: 'a): t('a) => value |> Rationale.Result.return |> Repromise.resolved;

let fromJsPromise = (promise: Js.Promise.t('a)): t('a) =>
  promise
  |> Repromise.Rejectable.fromJsPromise
  |> Repromise.Rejectable.map(result => Belt.Result.Ok(result))
  |> Repromise.Rejectable.catch(_ => Repromise.resolved(Belt.Result.Error()));

let toJsPromise = (promise: t('a)): Js.Promise.t('a) =>
  promise
  |> Repromise.Rejectable.relax
  |> Repromise.Rejectable.andThen(
       Rationale.Option.ofResult
       ||> Rationale.Option.map(Repromise.Rejectable.resolved)
       ||> OptionUtils.default(() => Repromise.Rejectable.rejected()),
     )
  |> Repromise.Rejectable.toJsPromise;

let fromRejectable = (promise: Repromise.Rejectable.t('a, _)): t('a) => promise |> Repromise.Rejectable.toJsPromise |> fromJsPromise;

let toRejectable = (promise: t('a)): Repromise.Rejectable.t('a, _) => promise |> toJsPromise |> Repromise.Rejectable.fromJsPromise;

let flatMapOk =
    (map: 'a => Belt.Result.t('b, unit), promise: Repromise.t(Belt.Result.t('a, unit))): Repromise.t(Belt.Result.t('b, unit)) =>
  promise |> Repromise.mapOk(map) |> Repromise.map(Rationale.Result.join);

let all = (promises: list(t('a))): t(list('a)) => promises |> List.map(toRejectable) |> Repromise.Rejectable.all |> fromRejectable;

let all2 = (promise1: t('a), promise2: t('b)): t(('a, 'b)) =>
  jsAll((promise1 |> toJsPromise, promise2 |> toJsPromise)) |> fromJsPromise;