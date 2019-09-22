open Rationale.Option.Infix;
open OptionUtils.Infix;

type script = {
  id: string,
  code: string,
};

type t = {
  newScriptCode: string,
  scripts: Selector.Optional.t(script),
};

type selectorItem = {
  script: option(script),
  name: React.element,
};

module Styles = {
  open Css;

  let section = style([marginTop(15 |> px), marginBottom(15 |> px)]);
};

[@react.component]
let make = (~value: t, ~onChange: t => unit) => {
  let newScript = {script: None, name: <Translation id="scriptWizard.newScript" />};
  let scripts: Selector.Required.t(selectorItem) =
    value.scripts
    |> Selector.Optional.mapi((i, script) => {script: Some(script), name: React.string("#" ++ string_of_int(i))})
    |> (
      ({before, selected, after}) =>
        switch (selected) {
        | Some(selected) => ({before: [newScript, ...before], selected, after}: Selector.Required.t(selectorItem))
        | None => {before: [], selected: newScript, after: before @ after}
        }
    );

  <div>
    <Select
      value={scripts |> Selector.toOptional}
      nameMapper={script => script.name}
      onChange={({selected}) =>
        onChange({
          ...value,
          scripts:
            selected
            >>= (x => x.script)
            <$> (selectedScript => value.scripts |> Selector.Optional.select(Rationale.Util.identical(selectedScript)))
            ||? value.scripts,
        })
      }
    />
    <CodeEditor
      value={scripts.selected.script <$> (x => x.code) ||? value.newScriptCode}
      onChange={code => onChange({newScriptCode: code, scripts: value.scripts |> Selector.Optional.unselect})}
    />
  </div>;
};