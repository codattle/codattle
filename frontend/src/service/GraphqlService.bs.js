// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Fetch = require("bs-fetch/src/Fetch.js");
var Js_dict = require("bs-platform/lib/js/js_dict.js");
var Js_json = require("bs-platform/lib/js/js_json.js");
var Repromise = require("@aantron/repromise/src/js/repromise.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");

function executeQuery(grapqhQuery) {
  return Repromise.Rejectable[/* catch */7]((function (_error) {
                return Repromise.resolved(undefined);
              }), Repromise.Rejectable[/* map */5]((function (optionalJson) {
                    if (optionalJson !== undefined) {
                      var match = Js_json.decodeObject(Caml_option.valFromOption(optionalJson));
                      if (match !== undefined) {
                        return Caml_option.some(Curry._1(grapqhQuery.parse, Caml_option.valFromOption(match)["data"]));
                      } else {
                        return undefined;
                      }
                    }
                    
                  }), Repromise.Rejectable[/* andThen */4]((function (response) {
                        if (response.ok) {
                          return Repromise.Rejectable[/* map */5]((function (json) {
                                        return Caml_option.some(json);
                                      }), Repromise.Rejectable[/* fromJsPromise */10](response.json()));
                        } else {
                          return Repromise.Rejectable[/* resolved */2](undefined);
                        }
                      }), Repromise.Rejectable[/* fromJsPromise */10](fetch(process.env.GRAPHQL_URL, Fetch.RequestInit[/* make */0](/* Post */2, /* array */[/* tuple */[
                                      "content-type",
                                      "application/json"
                                    ]], Caml_option.some(JSON.stringify(Js_dict.fromList(/* :: */[
                                                /* tuple */[
                                                  "query",
                                                  grapqhQuery.query
                                                ],
                                                /* :: */[
                                                  /* tuple */[
                                                    "variables",
                                                    grapqhQuery.variables
                                                  ],
                                                  /* [] */0
                                                ]
                                              ]))), undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined)(/* () */0))))));
}

exports.executeQuery = executeQuery;
/* Repromise Not a pure module */