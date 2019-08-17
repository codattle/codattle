[@react.component]
let make = (~image, ~width, ~height) => {
  let (imageAsUrl, setImageAsUrl) = React.useState(() => "");
  React.useEffect1(
    () => {
      let abort = FileReader.convertFileToDataUrl(image, imageAsUrl => setImageAsUrl(_ => imageAsUrl));
      Some(() => abort());
    },
    [|image|],
  );

  <img src=imageAsUrl width height />;
};