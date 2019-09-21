open Rationale.RList;

let useUrlPath = () => {
  let url = ReasonReactRouter.useUrl();
  "/" ++ (url.path |> join("/"));
};