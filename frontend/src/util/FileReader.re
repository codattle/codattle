type t;

type onloadEvent = {. "target": {. "result": string}};

[@bs.new] external create: unit => t = "FileReader";
[@bs.set] external setOnload: (t, onloadEvent => unit) => unit = "onload";
[@bs.send.pipe: t] external readAsDataUrl: File.t => unit = "readAsDataURL";
[@bs.send.pipe: t] external abort: unit => unit = "abort";

let convertFileToDataUrl = (file: File.t, onComplete: string => unit): (unit => unit) => {
  let reader = create();
  reader->setOnload(event => onComplete(event##target##result));
  reader |> readAsDataUrl(file);
  () => reader |> abort();
};