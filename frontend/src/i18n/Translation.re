[@react.component]
let make = (~id) => {
  ReasonReact.element(
    ReactIntl.FormattedMessage.make(
      ~id,
      ~defaultMessage="",
      [||],
    ),
  );
};