open Rationale.Option;
open Rationale.Option.Infix;
open Rationale.RList;

type languageMap = {
  language: Language.languageName,
  content: string,
};

type game = {
  id: string,
  description: list(languageMap),
};

type mode =
  | Editing
  | Joining
  | Failure;

module GetMatchQuery = [%graphql
  {|
  query($matchId: ID!) {
    match_: match(matchId: $matchId) {
      game {
        id,
        description {
          language,
          content
        }
      }
    }
  }
|}
];

module SendScriptMutation = [%graphql
  {|
  mutation($gameId: ID!, $code: String!) {
    sendScript(gameId: $gameId, code: $code) {
      id
    }
  }
|}
];

module JoinMatchMutation = [%graphql
  {|
  mutation($matchId: ID!, $scriptId: ID!) {
    joinMatch(matchId: $matchId, scriptId: $scriptId)
  }
|}
];

module Styles = {
  open Css;

  let standardPadding = 15 |> px;

  let header = style([fontWeight(`bold)]);
  let section = style([marginTop(standardPadding), marginBottom(standardPadding)]);
  let noDescription = style([fontStyle(`italic)]);
};

let findWithLanguage = (languageName: Language.languageName, descriptions: list(languageMap)) => {
  descriptions |> find(x => x.language === languageName);
};

let descriptionContentOrDefault = (languageName: Language.languageName, descriptions: list(languageMap)) => {
  findWithLanguage(languageName, descriptions)
  |? findWithLanguage(Language.defaultLanguage.name, descriptions)
  |? head(descriptions)
  |> fmap(languageMap => ReasonReact.string(languageMap.content))
  |> default(<span className=Styles.noDescription> <Translation id="scriptWizard.noDescription" /> </span>);
};

let descriptionHeader = {
  <span className=Styles.header> <Translation id="scriptWizard.description" /> </span>;
};

[@react.component]
let make = (~matchId: string) => {
  let language = React.useContext(LanguageContext.context);
  let (script, setScript) = React.useState(() => "");
  let (mode, setMode) = React.useState(() => Editing);

  let game =
    Utils.useResource(GetMatchQuery.make(~matchId, ()), [|matchId|], data =>
      {
        id: data##match##game##id,
        description: data##match##game##description |> Array.map(x => {language: x##language, content: x##content}) |> Array.to_list,
      }
    );

  let joinMatchWithNewScript = (script: string, gameId: string, matchId: string) => {
    setMode(_ => Joining);
    GraphqlService.executeQuery(SendScriptMutation.make(~gameId, ~code=script, ()))
    |> Repromise.map(result => result->Belt.Option.map(result => result##sendScript))
    |> Repromise.andThen(createdScript =>
         createdScript->Belt.Option.mapWithDefault(Repromise.resolved(None), createdScript =>
           GraphqlService.executeQuery(JoinMatchMutation.make(~matchId, ~scriptId=createdScript##id, ()))
         )
       )
    |> Repromise.wait(result =>
         switch (result) {
         | Some(_) => ReasonReactRouter.push("/games/matches/" ++ matchId)
         | None => setMode(_ => Failure)
         }
       );
  };

  switch (game) {
  | NotLoaded => <div />
  | Loading => <div> <Translation id="scriptWizard.loadingGame" /> </div>
  | Loaded(game) =>
    switch (mode) {
    | Editing =>
      Styles.(
        <div className=section>
          <div className=section>
            <ExpansionPanel header=descriptionHeader content={descriptionContentOrDefault(language.name, game.description)} />
          </div>
          <div className=section> <ScriptEditor value=script onChange={script => setScript(_ => script)} /> </div>
          <Button onClick={_ => joinMatchWithNewScript(script, game.id, matchId)}> <Translation id="scriptWizard.joinMatch" /> </Button>
        </div>
      )
    | Joining => <div> <Translation id="scriptWizard.joiningMatch" /> </div>
    | Failure => <div> {ReasonReact.string("Error while joining game :(")} </div>
    }
  | Failure => <div> {ReasonReact.string("Error while loading game :(")} </div>
  };
};