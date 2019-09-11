package com.codattle.core.resolver

import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Match
import com.codattle.core.model.MatchResult
import com.codattle.core.model.Script
import com.codattle.core.service.GameService
import com.codattle.core.service.MatchService
import com.codattle.core.service.ScriptService
import com.coxautodev.graphql.tools.GraphQLQueryResolver
import javax.inject.Singleton

@Singleton
class Query(private val gameService: GameService,
            private val matchService: MatchService,
            private val scriptService: ScriptService) : GraphQLQueryResolver {

    fun game(gameId: Id<Game>): Game {
        return gameService.getGame(gameId)
    }

    fun games(): List<Game> {
        return gameService.getGames()
    }

    fun searchGames(name: String?): List<Game> {
        return gameService.searchGames(name)
    }

    fun match(matchId: Id<Match>): Match {
        return matchService.getMatch(matchId)
    }

    fun matches(gameId: Id<Game>): List<Match> {
        return matchService.getMatchesOfGame(gameId)
    }

    fun scripts(): List<Script> {
        return scriptService.getScripts()
    }

    fun result(matchId: Id<Match>): MatchResult? {
        return matchService.getResultOfMatch(matchId)
    }

}