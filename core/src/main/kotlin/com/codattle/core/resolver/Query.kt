package com.codattle.core.resolver

import com.codattle.core.dao.Id
import com.codattle.core.dto.MatchWithGameDTO
import com.codattle.core.dto.MatchWithScriptsDTO
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

    fun games(): List<Game> {
        return gameService.getGames()
    }

    fun match(matchId: Id<Match>): MatchWithGameDTO {
        val match = matchService.getMatch(matchId)
        val game = gameService.getGame(match.game)
        val scripts = scriptService.getScripts(match.scripts)
        return MatchWithGameDTO(matchId, match.name, scripts, match.result, game)
    }

    fun matches(gameId: Id<Game>): List<MatchWithScriptsDTO> {
        return matchService.getMatches(gameId).map { MatchWithScriptsDTO.from(it, scriptService) }
    }

    fun scripts(): List<Script> {
        return scriptService.getScripts()
    }

    fun result(matchId: Id<Match>): MatchResult? {
        return matchService.getResultOfMatch(matchId)
    }

}