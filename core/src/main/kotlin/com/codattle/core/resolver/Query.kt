package com.codattle.core.resolver

import com.codattle.core.dto.MatchWithGameDTO
import com.codattle.core.model.Game
import com.codattle.core.model.Match
import com.codattle.core.model.Result
import com.codattle.core.model.Script
import com.codattle.core.service.GameService
import com.codattle.core.service.MatchService
import com.codattle.core.service.ScriptService
import com.coxautodev.graphql.tools.GraphQLQueryResolver
import org.bson.types.ObjectId
import javax.inject.Singleton

@Singleton
class Query(private val gameService: GameService,
            private val matchService: MatchService,
            private val scriptService: ScriptService) : GraphQLQueryResolver {

    fun games(): List<Game> {
        return gameService.getGames()
    }

    fun match(matchId: ObjectId): MatchWithGameDTO {
        val match = matchService.getMatch(matchId)
        val game = gameService.getGame(match.game?.id!!)
        return MatchWithGameDTO(match.id!!, match.name!!, match.scripts, match.result, game)
    }

    fun matches(gameId: ObjectId): List<Match> {
        return matchService.getMatches(gameId)
    }

    fun scripts(): List<Script> {
        return scriptService.getScripts()
    }

    fun result(matchId: ObjectId): Result {
        return matchService.getResultOfMatch(matchId)
    }

}