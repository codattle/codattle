type t;

[@bs.new] external create: unit => t = "Image";
[@bs.set] external setOnload: (t, unit => unit) => unit = "onload";
[@bs.set] external setSrc: (t, string) => unit = "src";

let loadFromUrl = url => {
  let (promise, resolve) = Promise.pending();
  let image = create();
  image->setOnload(() => resolve(image));
  image->setSrc(url);
  promise
};