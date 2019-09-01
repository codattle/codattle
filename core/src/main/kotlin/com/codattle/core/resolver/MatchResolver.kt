package com.codattle.core.resolver

import com.codattle.core.model.Game
import com.codattle.core.model.Match
import com.codattle.core.model.Script
import com.codattle.core.service.GameService
import com.codattle.core.service.ScriptService
import com.coxautodev.graphql.tools.GraphQLResolver
import javax.inject.Singleton

@Singleton
class MatchResolver(private val gameService: GameService, private val scriptService: ScriptService) : GraphQLResolver<Match> {

    fun game(match: Match): Game {
        return gameService.getGame(match.game)
    }

    fun scripts(match: Match): List<Script> {
        return scriptService.getScripts(match.scripts)
    }

    fun scriptsCount(match: Match): Int {
        return match.scripts.size
    }
}