package com.codattle.core.resolver

import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import com.codattle.core.service.GameService
import com.codattle.core.service.MatchService
import com.codattle.core.service.ScriptService
import com.codattle.core.service.TournamentService
import com.coxautodev.graphql.tools.GraphQLQueryResolver
import javax.inject.Singleton

@Singleton
class Query(private val gameService: GameService,
            private val matchService: MatchService,
            private val scriptService: ScriptService,
            private val tournamentService: TournamentService
) : GraphQLQueryResolver {

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

    fun script(scriptId: Id<Script>): Script {
        return scriptService.getScript(scriptId)
    }

    fun scripts(gameId: Id<Game>, authorId: Id<User>?): List<Script> {
        return scriptService.getScripts(gameId, authorId)
    }

    fun result(matchId: Id<Match>): MatchResult? {
        return matchService.getResultOfMatch(matchId)
    }

    fun tournament(tournamentId: Id<Tournament>): Tournament {
        return tournamentService.getTournament(tournamentId)
    }

    fun tournaments(gameId: Id<Game>): List<Tournament> {
        return tournamentService.getTournamentsOfGame(gameId)
    }
}