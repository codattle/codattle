// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Js_exn = require("bs-platform/lib/js/js_exn.js");
var Js_dict = require("bs-platform/lib/js/js_dict.js");
var Js_json = require("bs-platform/lib/js/js_json.js");
var Repromise = require("@aantron/repromise/src/js/repromise.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var GameCreator$ReactTemplate = require("./component/GameCreator.bs.js");
var GameSelector$ReactTemplate = require("./component/GameSelector.bs.js");
var ScriptEditor$ReactTemplate = require("./component/ScriptEditor.bs.js");
var MatchSelector$ReactTemplate = require("./component/MatchSelector.bs.js");
var GraphqlService$ReactTemplate = require("./service/GraphqlService.bs.js");

var component = ReasonReact.reducerComponent("App");

var ppx_printed_query = "mutation ($gameId: ID!, $content: String!)  {\nsendScript(gameId: $gameId, content: $content)  {\nid  \n}\n\n}\n";

function parse(value) {
  var match = Js_json.decodeObject(value);
  if (match !== undefined) {
    var match$1 = Js_dict.get(Caml_option.valFromOption(match), "sendScript");
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
            tmp$2 = Js_exn.raiseError("graphql_ppx: Field id on type Script is missing");
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
            sendScript: tmp
          };
  } else {
    return Js_exn.raiseError("graphql_ppx: Object is not a value");
  }
}

function make(gameId, content, param) {
  return {
          query: ppx_printed_query,
          variables: Js_dict.fromArray(/* array */[
                /* tuple */[
                  "gameId",
                  gameId
                ],
                /* tuple */[
                  "content",
                  content
                ]
              ]),
          parse: parse
        };
}

function makeWithVariables(variables) {
  var gameId = variables.gameId;
  var content = variables.content;
  return {
          query: ppx_printed_query,
          variables: Js_dict.fromArray(/* array */[
                /* tuple */[
                  "gameId",
                  gameId
                ],
                /* tuple */[
                  "content",
                  content
                ]
              ]),
          parse: parse
        };
}

function ret_type(f) {
  return /* module */[];
}

var MT_Ret = /* module */[];

var SendScriptMutation = /* module */[
  /* ppx_printed_query */ppx_printed_query,
  /* query */ppx_printed_query,
  /* parse */parse,
  /* make */make,
  /* makeWithVariables */makeWithVariables,
  /* ret_type */ret_type,
  /* MT_Ret */MT_Ret
];

var ppx_printed_query$1 = "mutation ($matchId: ID!, $scriptId: ID!)  {\njoinMatch(matchId: $matchId, scriptId: $scriptId)  \n}\n";

function parse$1(value) {
  var match = Js_json.decodeObject(value);
  if (match !== undefined) {
    var match$1 = Js_dict.get(Caml_option.valFromOption(match), "joinMatch");
    var tmp;
    if (match$1 !== undefined) {
      var value$1 = Caml_option.valFromOption(match$1);
      var match$2 = Js_json.decodeNull(value$1);
      if (match$2 !== undefined) {
        tmp = undefined;
      } else {
        var match$3 = Js_json.decodeBoolean(value$1);
        tmp = match$3 !== undefined ? match$3 : Js_exn.raiseError("graphql_ppx: Expected boolean, got " + JSON.stringify(value$1));
      }
    } else {
      tmp = undefined;
    }
    return {
            joinMatch: tmp
          };
  } else {
    return Js_exn.raiseError("graphql_ppx: Object is not a value");
  }
}

function make$1(matchId, scriptId, param) {
  return {
          query: ppx_printed_query$1,
          variables: Js_dict.fromArray(/* array */[
                /* tuple */[
                  "matchId",
                  matchId
                ],
                /* tuple */[
                  "scriptId",
                  scriptId
                ]
              ]),
          parse: parse$1
        };
}

function makeWithVariables$1(variables) {
  var matchId = variables.matchId;
  var scriptId = variables.scriptId;
  return {
          query: ppx_printed_query$1,
          variables: Js_dict.fromArray(/* array */[
                /* tuple */[
                  "matchId",
                  matchId
                ],
                /* tuple */[
                  "scriptId",
                  scriptId
                ]
              ]),
          parse: parse$1
        };
}

function ret_type$1(f) {
  return /* module */[];
}

var MT_Ret$1 = /* module */[];

var JoinMatchMutation = /* module */[
  /* ppx_printed_query */ppx_printed_query$1,
  /* query */ppx_printed_query$1,
  /* parse */parse$1,
  /* make */make$1,
  /* makeWithVariables */makeWithVariables$1,
  /* ret_type */ret_type$1,
  /* MT_Ret */MT_Ret$1
];

function joinMatch(script, gameId, matchId) {
  return Repromise.wait((function (param) {
                return /* () */0;
              }), Repromise.andThen((function (result) {
                    return Belt_Option.mapWithDefault(Belt_Option.flatMap(result, (function (result) {
                                      return result.sendScript;
                                    })), Repromise.resolved(undefined), (function (script) {
                                  return GraphqlService$ReactTemplate.executeQuery(make$1(matchId, script.id, /* () */0));
                                }));
                  }), GraphqlService$ReactTemplate.executeQuery(make(gameId, script, /* () */0))));
}

function make$2(_children) {
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
              var state = param[/* state */1];
              var match = state[/* selectedGameId */0];
              var tmp;
              if (match !== undefined) {
                var gameId = match;
                var match$1 = state[/* selectedMatchId */1];
                var tmp$1;
                if (match$1 !== undefined) {
                  var matchId = match$1;
                  tmp$1 = React.createElement(React.Fragment, undefined, ReasonReact.element(undefined, undefined, ScriptEditor$ReactTemplate.make((function (script) {
                                  return Curry._1(send, /* SetScript */Block.__(2, [script]));
                                }), /* array */[])), React.createElement("button", {
                            onClick: (function (param) {
                                return joinMatch(state[/* script */2], gameId, matchId);
                              })
                          }, "Join match"));
                } else {
                  tmp$1 = React.createElement(React.Fragment, undefined);
                }
                tmp = React.createElement(React.Fragment, undefined, ReasonReact.element(undefined, undefined, MatchSelector$ReactTemplate.make(gameId, (function (matchId) {
                                return Curry._1(send, /* SelectMatch */Block.__(1, [matchId]));
                              }), /* array */[])), tmp$1);
              } else {
                tmp = React.createElement(React.Fragment, undefined);
              }
              return React.createElement("div", undefined, React.createElement("h1", undefined, "Welcome to Codattle"), ReasonReact.element(undefined, undefined, GameCreator$ReactTemplate.make(/* array */[])), ReasonReact.element(undefined, undefined, GameSelector$ReactTemplate.make((function (gameId) {
                                    return Curry._1(send, /* SelectGame */Block.__(0, [gameId]));
                                  }), /* array */[])), tmp);
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* selectedGameId */undefined,
                      /* selectedMatchId */undefined,
                      /* script */""
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              switch (action.tag | 0) {
                case 0 : 
                    return /* Update */Block.__(0, [/* record */[
                                /* selectedGameId */action[0],
                                /* selectedMatchId */undefined,
                                /* script */state[/* script */2]
                              ]]);
                case 1 : 
                    return /* Update */Block.__(0, [/* record */[
                                /* selectedGameId */state[/* selectedGameId */0],
                                /* selectedMatchId */action[0],
                                /* script */state[/* script */2]
                              ]]);
                case 2 : 
                    return /* Update */Block.__(0, [/* record */[
                                /* selectedGameId */state[/* selectedGameId */0],
                                /* selectedMatchId */state[/* selectedMatchId */1],
                                /* script */action[0]
                              ]]);
                
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.SendScriptMutation = SendScriptMutation;
exports.JoinMatchMutation = JoinMatchMutation;
exports.joinMatch = joinMatch;
exports.make = make$2;
/* component Not a pure module */