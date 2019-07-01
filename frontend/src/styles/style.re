open Css;

module Color = {
  module ColorLib = {
    type t;

    [@bs.module "color"] [@bs.val] external fromHexString: string => t = "rgb";
    [@bs.send.pipe: t] external toHexString: string = "hex";
    [@bs.send.pipe: t] external lighten: float => t = "lighten";
    [@bs.send.pipe: t] external darken: float => t = "darken";

    let dropFirstCharacter = string => {
      String.sub(string, 1, String.length(string) - 1);
    };

    let fromHex = hex =>
      switch (hex) {
      | `hex(hexString) => "#" ++ hexString |> fromHexString
      };

    let toHex = color => color |> toHexString |> dropFirstCharacter |> hex;
  };

  let darken = (ratio, color) => ColorLib.(color |> fromHex |> darken(ratio) |> toHex);

  let textColor = white;
  let primaryColor = hex("2ADB82");
};