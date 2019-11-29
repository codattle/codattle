package com.codattle.core.resolver

import com.codattle.core.model.Game
import com.codattle.core.model.Match
import com.codattle.core.model.Script
import com.codattle.core.model.Tournament
import com.codattle.core.service.GameService
import com.codattle.core.service.MatchService
import com.codattle.core.service.ScriptService
import com.coxautodev.graphql.tools.GraphQLResolver
import javax.inject.Singleton

@Singleton
class TournamentResolver(
        private val gameService: GameService,
        private val scriptService: ScriptService,
        private val matchService: MatchService
) : GraphQLResolver<Tournament> {

    fun game(tournament: Tournament): Game {
        return gameService.getGame(tournament.game)
    }

    fun scripts(tournament: Tournament): List<Script> {
        return scriptService.getScripts(tournament.scripts)
    }

    fun matches(tournament: Tournament): List<Match>? {
        return tournament.matches?.let { matchService.getMatches(it) }
    }
}