package com.codattle.core.service

import com.codattle.core.dao.GameDao
import com.codattle.core.dao.MatchDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import java.lang.IllegalArgumentException
import javax.inject.Singleton

@Singleton
class MatchService(private val matchDao: MatchDao, private val gameDao: GameDao, private val queueService: QueueService) {

    companion object {
        private const val SIMULATION_QUEUE = "simulation"
    }

    init {
        queueService.createQueue(SIMULATION_QUEUE)
    }

    fun getMatch(matchId: Id<Match>): Match {
        return matchDao.getMatch(matchId)
    }

    fun getMatches(matchIds: List<Id<Match>>): List<Match> {
        return if (matchIds.isEmpty()) {
            emptyList()
        } else {
            val matches = matchDao.getMatches(matchIds).map { it.id to it }.toMap()
            matchIds.map { matches[it] ?: throw IllegalArgumentException("Match with id \"$it\" doesn't exist.") }
        }
    }

    fun getMatchesOfGame(gameId: Id<Game>): List<Match> {
        return matchDao.getMatchesOfGame(gameId)
    }

    fun getResultOfMatch(matchId: Id<Match>): MatchResult? {
        return matchDao.getResultOfMatch(matchId)
    }

    fun createMatch(name: String, gameId: Id<Game>): Match {
        return matchDao.saveMatch(Match.builder().name(name).game(gameId))
    }

    fun createInstantMatch(gameId: Id<Game>, scripts: List<Id<Script>>): Match {
        require(gameDao.isPlayersCountAllowed(gameId, scripts.size)) { "Cannot create instant match to this game with ${scripts.size} scripts" }

        val match = matchDao.saveMatch(Match.builder().name("").game(gameId).scripts(scripts))
        startMatch(match.id)
        return match
    }

    fun joinMatch(matchId: Id<Match>, scriptId: Id<Script>) {
        var match = getMatch(matchId)
        val game = gameDao.getGame(match.game)

        require(canJoinMatch(match, game)) { "Cannot join to match with id $matchId" }

        match = matchDao.addScriptIfScriptCountEquals(matchId, scriptId, match.scripts.size)
                ?: throw IllegalStateException("Scripts count have changed while joining. Probably race occurred.")

        if (match.scripts.size == game.maxAllowedPlayerCount) {
            startMatch(matchId)
        }
    }

    private fun canJoinMatch(match: Match, game: Game): Boolean {
        // TODO: Joining to started match should be impossible
        return match.scripts.size < game.maxAllowedPlayerCount
    }

    private fun startMatch(matchId: Id<Match>) {
        queueService.sendMessage(SIMULATION_QUEUE, matchId.value.toByteArray())
    }

    fun provideResultFrames(matchId: Id<Match>, resultFrames: List<ResultFrame>) {
        matchDao.provideResultFrames(matchId, resultFrames)
    }

    fun provideMatchWinner(matchId: Id<Match>, winner: Int) {
        matchDao.provideMatchWinner(matchId, winner)
    }
}