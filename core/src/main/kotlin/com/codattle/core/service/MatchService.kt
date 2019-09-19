package com.codattle.core.service

import com.codattle.core.dao.GameDao
import com.codattle.core.dao.MatchDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
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
        val game = gameDao.getGame(gameId)

        return matchDao.saveMatch(Match.builder().name(name).game(gameId))
    }

    fun joinMatch(matchId: Id<Match>, scriptId: Id<Script>) {
        var match = getMatch(matchId)
        val game = gameDao.getGame(match.game)

        require(canJoinMatch(match, game)) { "Cannot join to match with id $matchId" }

        match = matchDao.addScriptIfScriptsCountEquals(matchId, scriptId, match.scripts.size)
                ?: throw IllegalStateException("Scripts count have changed while joining. Probably race occurred.")

        if (match.scripts.size == game.maxAllowedPlayerCount) {
            startMatch(matchId)
        }
    }

    private fun canJoinMatch(match: Match, game: Game): Boolean {
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