// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Js_exn = require("bs-platform/lib/js/js_exn.js");
var Js_dict = require("bs-platform/lib/js/js_dict.js");
var Js_json = require("bs-platform/lib/js/js_json.js");
var Repromise = require("@aantron/repromise/src/js/repromise.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var ScriptEditor$ReactTemplate = require("./ScriptEditor.bs.js");
var GraphqlService$ReactTemplate = require("../service/GraphqlService.bs.js");

var component = ReasonReact.reducerComponent("GameCreator");

var ppx_printed_query = "mutation ($name: String!, $mainLoop: String!)  {\ncreateGame(name: $name, mainLoop: $mainLoop)  {\nid  \n}\n\n}\n";

function parse(value) {
  var match = Js_json.decodeObject(value);
  if (match !== undefined) {
    var match$1 = Js_dict.get(Caml_option.valFromOption(match), "createGame");
    var tmp;
    if (match$1 !== undefined) {
      var value$1 = Caml_option.valFromOption(match$1);
      var match$2 = Js_json.decodeNull(value$1);
      if (match$2 !== undefined) {
        tmp = undefined;
      } else {
        var match$3 = Js_json.decodeObject(value$1);
        var tmp$1;
        if (match$3 !== undefined) {
          var match$4 = Js_dict.get(Caml_option.valFromOption(match$3), "id");
          var tmp$2;
          if (match$4 !== undefined) {
            var value$2 = Caml_option.valFromOption(match$4);
            var match$5 = Js_json.decodeString(value$2);
            tmp$2 = match$5 !== undefined ? match$5 : Js_exn.raiseError("graphql_ppx: Expected string, got " + JSON.stringify(value$2));
          } else {
            tmp$2 = Js_exn.raiseError("graphql_ppx: Field id on type Game is missing");
          }
          tmp$1 = {
            id: tmp$2
          };
        } else {
          tmp$1 = Js_exn.raiseError("graphql_ppx: Object is not a value");
        }
        tmp = Caml_option.some(tmp$1);
      }
    } else {
      tmp = undefined;
    }
    return {
            createGame: tmp
          };
  } else {
    return Js_exn.raiseError("graphql_ppx: Object is not a value");
  }
}

function make(name, mainLoop, param) {
  return {
          query: ppx_printed_query,
          variables: Js_dict.fromArray(/* array */[
                /* tuple */[
                  "name",
                  name
                ],
                /* tuple */[
                  "mainLoop",
                  mainLoop
                ]
              ]),
          parse: parse
        };
}

function makeWithVariables(variables) {
  var name = variables.name;
  var mainLoop = variables.mainLoop;
  return {
          query: ppx_printed_query,
          variables: Js_dict.fromArray(/* array */[
                /* tuple */[
                  "name",
                  name
                ],
                /* tuple */[
                  "mainLoop",
                  mainLoop
                ]
              ]),
          parse: parse
        };
}

function ret_type(f) {
  return /* module */[];
}

var MT_Ret = /* module */[];

var CreateGameMutation = /* module */[
  /* ppx_printed_query */ppx_printed_query,
  /* query */ppx_printed_query,
  /* parse */parse,
  /* make */make,
  /* makeWithVariables */makeWithVariables,
  /* ret_type */ret_type,
  /* MT_Ret */MT_Ret
];

function createGame(name, script) {
  return Repromise.wait((function (param) {
                return /* () */0;
              }), GraphqlService$ReactTemplate.executeQuery(make(name, script, /* () */0)));
}

function make$1(_children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */component[/* didMount */4],
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (param) {
              var send = param[/* send */3];
              return React.createElement("div", undefined, React.createElement("button", {
                              onClick: (function (_event) {
                                  return Curry._1(send, /* CreateGame */0);
                                })
                            }, "Create game"), React.createElement("div", undefined, React.createElement("label", {
                                  htmlFor: "gameName"
                                }, "Name"), React.createElement("input", {
                                  id: "gameName",
                                  value: param[/* state */1][/* name */0],
                                  onChange: (function ($$event) {
                                      return Curry._1(send, /* SetName */Block.__(0, [$$event.target.value]));
                                    })
                                })), ReasonReact.element(undefined, undefined, ScriptEditor$ReactTemplate.make((function (script) {
                                    return Curry._1(send, /* SetScript */Block.__(1, [script]));
                                  }), /* array */[])));
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* name */"",
                      /* script */""
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              if (typeof action === "number") {
                return /* SideEffects */Block.__(1, [(function (_self) {
                              return createGame(state[/* name */0], state[/* script */1]);
                            })]);
              } else if (action.tag) {
                return /* Update */Block.__(0, [/* record */[
                            /* name */state[/* name */0],
                            /* script */action[0]
                          ]]);
              } else {
                return /* Update */Block.__(0, [/* record */[
                            /* name */action[0],
                            /* script */state[/* script */1]
                          ]]);
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.CreateGameMutation = CreateGameMutation;
exports.createGame = createGame;
exports.make = make$1;
/* component Not a pure module */