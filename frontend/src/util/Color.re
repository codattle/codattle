open Rationale.Function;

type t;

[@bs.module "color"] [@bs.val] external fromHexString: string => t = "rgb";
[@bs.send.pipe: t] external toHexString: string = "hex";
[@bs.send.pipe: t] external lighten: float => t = "lighten";
[@bs.send.pipe: t] external darken: float => t = "darken";

let dropFirstCharacter = string => {
  String.sub(string, 1, String.length(string) - 1);
};

let hexToHexString = (hex: [> `hex(string)]) => {
  switch (hex) {
  | `hex(hexString) => "#" ++ hexString
  };
};

let hexStringToHex = hexString => `hex(hexString |> dropFirstCharacter)

let fromHex = hexToHexString ||> fromHexString;

let toHex = toHexString ||> hexStringToHex;

let black = `hex("000000");