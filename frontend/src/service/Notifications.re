module Provider = {
  [@bs.module "react-toast-notifications"] [@react.component] external make: (~children: 'a=?) => React.element = "ToastProvider";
};

module Binding = {
  type toastProps = {
    .
    "appearance": string,
    "autoDismiss": Js.undefined(bool),
  };

  [@bs.module "react-toast-notifications"] [@bs.val]
  external useToasts: unit => {. [@bs.meth] "addToast": (React.element, toastProps) => unit} = "useToasts";
};

open Binding;

[@bs.deriving jsConverter]
type appearance = [ | [@bs.as "success"] `Success | [@bs.as "info"] `Info | [@bs.as "warning"] `Warning | [@bs.as "error"] `Error];

type t = {showNotification: (string, appearance) => unit};

let useNotifications = () => {
  let toasts = useToasts();
  {
    showNotification: (message, appearance) =>
      toasts##addToast(
        <Translation id=message />,
        {"appearance": appearanceToJs(appearance), "autoDismiss": Js.Undefined.return(true)},
      ),
  };
};