open Css;

module Color = {
  let darken = (ratio, color) => Color.(color |> fromHex |> darken(ratio) |> toHex);

  let textColor = white;
  let primaryColor = hex("2ADB82");
};