let all2 = (promise1, promise2) => {
  let (promise, resolve, reject) = Repromise.Rejectable.make();

  let result1 = ref(None);
  let result2 = ref(None);

  let handlePromise = (promise, promiseResult) => {
    promise
    |> Repromise.Rejectable.catch(error => {
         reject(error);
         Repromise.Rejectable.rejected();
       })
    |> Repromise.Rejectable.wait(result => {
         promiseResult := Some(result);
         (result1^, result2^) |> OptionUtils.ifSome2((result1, result2) => resolve((result1, result2)));
       });
  };

  handlePromise(promise1, result1);
  handlePromise(promise2, result2);

  promise;
};