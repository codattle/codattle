package com.codattle.core.service

import com.codattle.core.dao.TournamentDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import javax.inject.Singleton

@Singleton
class TournamentService(
        private val tournamentDao: TournamentDao,
        private val gameService: GameService,
        private val matchService: MatchService
) {

    companion object {
        private const val MAX_SCRIPTS_COUNT_IN_TOURNAMENT = 8
        private const val PLAYER_COUNT_PER_MATCH = 2
    }

    fun getTournament(tournamentId: Id<Tournament>): Tournament {
        return tournamentDao.getTournament(tournamentId)
    }

    fun getTournamentsOfGame(gameId: Id<Game>): List<Tournament> {
        return tournamentDao.getTournamentsOfGame(gameId)
    }

    fun createTournament(name: String, gameId: Id<Game>, maxScriptCount: Int): Tournament {
        require(maxScriptCount in PLAYER_COUNT_PER_MATCH..MAX_SCRIPTS_COUNT_IN_TOURNAMENT) {
            "Count of scripts in tournament must be between $PLAYER_COUNT_PER_MATCH and $MAX_SCRIPTS_COUNT_IN_TOURNAMENT"
        }
        require(gameService.doesGameAllowPlayerCount(gameId, PLAYER_COUNT_PER_MATCH)) {
            "Game must allow to create match for $PLAYER_COUNT_PER_MATCH players"
        }

        return tournamentDao.saveTournament(Tournament.builder()
                .name(name)
                .game(gameId)
                .maxScriptCount(maxScriptCount))
    }

    fun joinTournament(tournamentId: Id<Tournament>, scriptId: Id<Script>) {
        var tournament = getTournament(tournamentId)

        require(canJoinTournament(tournament)) { "Cannot join to tournament with id $tournamentId" }

        tournament = tournamentDao.addScriptIfScriptCountEquals(tournamentId, scriptId, tournament.scripts.size)
                ?: throw IllegalStateException("Scripts count have changed while joining. Probably race occurred.")

        if (tournament.scripts.size == tournament.maxScriptCount) {
            startTournament(tournament)
        }
    }

    private fun canJoinTournament(tournament: Tournament): Boolean {
        return tournament.scripts.size < tournament.maxScriptCount
    }

    private fun startTournament(tournament: Tournament) {
        val matches = getPairsOfScriptsInRoundRobinSystem(tournament.scripts)
                .map {
                    // TODO: matches in tournament shouldn't be shown in match list
                    val matchName = "Tournament " + tournament.name + ": " + it.first.value + " vs " + it.second.value
                    val match = matchService.createInstantMatch(tournament.game, listOf(it.first, it.second), matchName)

                    match.id
                }

        tournamentDao.setMatches(tournament.id, matches)
    }

    private fun getPairsOfScriptsInRoundRobinSystem(scripts: List<Id<Script>>): List<Pair<Id<Script>, Id<Script>>> {
        val result = mutableListOf<Pair<Id<Script>, Id<Script>>>()

        for (i in 0 until (scripts.size - 1)) {
            for (j in (i + 1) until scripts.size) {
                result.add(scripts[i] to scripts[j])
            }
        }

        return result
    }
}