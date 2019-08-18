package com.codattle.core.resolver

import com.codattle.core.dao.common.Id
import com.codattle.core.dto.MatchWithScriptsDTO
import com.codattle.core.model.*
import com.codattle.core.service.GameService
import com.codattle.core.service.MatchService
import com.codattle.core.service.ScriptService
import com.coxautodev.graphql.tools.GraphQLMutationResolver
import javax.inject.Singleton

@Singleton
class Mutation(private val gameService: GameService,
               private val matchService: MatchService,
               private val scriptService: ScriptService) : GraphQLMutationResolver {

    fun createGame(name: String, description: String, code: String, logo: Id<File>?, sprites: List<Sprite>?): Game {
        return gameService.createGame(name, description, code, logo, sprites ?: listOf())
    }

    fun addSpriteToGame(gameId: Id<Game>, sprite: Sprite): Boolean {
        gameService.addSprite(gameId, sprite)
        return true
    }

    fun createMatch(name: String, gameId: Id<Game>): MatchWithScriptsDTO {
        val match = matchService.createMatch(name, gameId)
        return MatchWithScriptsDTO.from(match, scriptService)
    }

    fun sendScript(gameId: Id<Game>, content: String): Script {
        return scriptService.createScript(gameId, content)
    }

    fun joinMatch(matchId: Id<Match>, scriptId: Id<Script>): Boolean {
        matchService.joinMatch(matchId, scriptId)
        return true
    }

    fun provideResultFrames(matchId: Id<Match>, resultFrames: List<ResultFrame>): Boolean {
        matchService.provideResultFrames(matchId, resultFrames)
        return true
    }

    fun provideMatchWinner(matchId: Id<Match>, winner: Int): Boolean {
        matchService.provideMatchWinner(matchId, winner)
        return true
    }
}