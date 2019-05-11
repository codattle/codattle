package com.codattle.core.resolver

import com.codattle.core.dto.NewResultFrameDTO
import com.codattle.core.model.Game
import com.codattle.core.model.Match
import com.codattle.core.model.Script
import com.codattle.core.service.GameService
import com.codattle.core.service.MatchService
import com.codattle.core.service.ScriptService
import com.coxautodev.graphql.tools.GraphQLMutationResolver
import org.bson.types.ObjectId
import javax.inject.Singleton

@Singleton
class Mutation(private val gameService: GameService,
               private val matchService: MatchService,
               private val scriptService: ScriptService) : GraphQLMutationResolver {

    fun createGame(name: String, mainLoop: String): Game {
        return gameService.createGame(name, mainLoop)
    }

    fun createMatch(name: String, gameId: ObjectId): Match {
        return matchService.createMatch(name, gameId)
    }

    fun sendScript(gameId: ObjectId, content: String): Script {
        return scriptService.createScript(gameId, content)
    }

    fun joinMatch(matchId: ObjectId, scriptId: ObjectId): Boolean {
        matchService.joinMatch(matchId, scriptId)
        return true
    }

    fun provideResultFrames(matchId: ObjectId, resultFrames: List<NewResultFrameDTO>): Boolean {
        matchService.provideResultFrames(matchId, resultFrames.map { it.toModel() })
        return true
    }
}