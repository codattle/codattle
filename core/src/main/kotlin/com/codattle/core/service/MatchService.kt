package com.codattle.core.service

import com.codattle.core.dao.MatchDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import java.lang.IllegalStateException
import javax.inject.Singleton

@Singleton
class MatchService(private val matchDao: MatchDao, private val queueService: QueueService) {

    companion object {
        private const val SIMULATION_QUEUE = "simulation"
        private const val SCRIPTS_PER_MATCH = 2 // TODO: later we can support games for one or more than two players
    }

    init {
        queueService.createQueue(SIMULATION_QUEUE)
    }

    fun getMatch(matchId: Id<Match>): Match {
        return matchDao.getMatch(matchId)
    }

    fun getMatchesOfGame(gameId: Id<Game>): List<Match> {
        return matchDao.getMatchesOfGame(gameId)
    }

    fun getResultOfMatch(matchId: Id<Match>): MatchResult? {
        return matchDao.getResultOfMatch(matchId)
    }

    fun createMatch(name: String, gameId: Id<Game>): Match {
        return matchDao.saveMatch(Match.Builder(name = name, game = gameId))
    }

    fun joinMatch(matchId: Id<Match>, scriptId: Id<Script>) {
        var match = getMatch(matchId)

        if (!canJoinMatch(match)) {
            throw IllegalArgumentException("Cannot join to match with id $matchId")
        }

        match = matchDao.addScriptIfScriptsCountEquals(matchId, scriptId, match.scripts.size)
                ?: throw IllegalStateException("Scripts count have changed while joining. Probably race occurred.")

        if (match.scripts.size == SCRIPTS_PER_MATCH) {
            startMatch(matchId)
        }
    }

    private fun canJoinMatch(match: Match): Boolean {
        return match.scripts.size < SCRIPTS_PER_MATCH
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