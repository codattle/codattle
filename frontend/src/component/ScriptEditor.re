type state = {text: string};

type action =
  | ChangeText(string);

let component = ReasonReact.reducerComponent("ScriptEditor");

let make = (~onChange=?, _children) => {
  ...component,
  initialState: () => {text: ""},
  reducer: (action, _state) =>
    switch (action) {
    | ChangeText(text) =>
      ReasonReact.UpdateWithSideEffects(
        {text: text},
        _ =>
          switch (onChange) {
          | Some(onChange) => onChange(text)
          | None => ()
          },
      )
    },
  render: ({state, send}) => {
    <textarea value={state.text} onChange={event => send(ChangeText(ReactEvent.Form.target(event)##value))} />;
  },
};