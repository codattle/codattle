package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.dao.Id
import com.codattle.core.model.*
import org.litote.kmongo.*
import javax.inject.Singleton

@Singleton
class MatchService(private val dao: Dao, private val queueService: QueueService) {

    companion object {
        private const val SIMULATION_QUEUE = "simulation"
        private const val SCRIPTS_PER_MATCH = 2 // TODO: later we can support games for one or more than two players
    }

    init {
        queueService.createQueue(SIMULATION_QUEUE)
    }

    fun getMatch(matchId: Id<Match>): Match {
        return dao.get(matchId, Match::class.java)
                ?: throw IllegalArgumentException("Match with id \"$matchId\" doesn't exist.")
    }

    fun getMatches(gameId: Id<Game>): List<Match> {
        return dao.getMany(Match::class.java, Match::game eq gameId)
    }

    fun getResultOfMatch(matchId: Id<Match>): MatchResult? {
        return dao.getWithFields(matchId, Match::class.java, Match::result)
                ?: throw IllegalArgumentException("Match with id \"$matchId\" doesn't exist.")
    }

    fun createMatch(name: String, gameId: Id<Game>): Match {
        return dao.save(Match(name = name, game = gameId))
    }

    // TODO: should be atomic
    fun joinMatch(matchId: Id<Match>, scriptId: Id<Script>) {
        var match = getMatch(matchId)
        if (!canJoinMatch(match)) {
            throw IllegalArgumentException("Cannot join to match with id $matchId")
        }
        match = match.copy(scripts = match.scripts + scriptId)
        dao.save(match)

        if (match.scripts.size == SCRIPTS_PER_MATCH) {
            startMatch(matchId)
        }
    }

    private fun canJoinMatch(match: Match): Boolean {
        return match.scripts.size < SCRIPTS_PER_MATCH
    }

    private fun startMatch(matchId: Id<Match>) {
        queueService.sendMessage(SIMULATION_QUEUE, matchId.id.toByteArray())
    }

    fun provideResultFrames(matchId: Id<Match>, resultFrames: List<ResultFrame>) {
        createEmptyMatchResultIfDoesNotExist(matchId)
        dao.findAndModify(Match::class.java, Match::id eq matchId, pushEach(Match::result / MatchResult::resultFrames, resultFrames))
    }

    fun provideMatchWinner(matchId: Id<Match>, winner: Int) {
        createEmptyMatchResultIfDoesNotExist(matchId)
        val match = dao.findAndModify(
                Match::class.java,
                and(Match::id eq matchId, (Match::result / MatchResult::winner) eq (null as Int?)),
                setValue(Match::result / MatchResult::winner, winner))
        match ?: throw IllegalArgumentException("Match with id $matchId doesn't exist or has already winner")
    }

    private fun createEmptyMatchResultIfDoesNotExist(matchId: Id<Match>) {
        dao.findAndModify(Match::class.java, and(Match::id eq matchId, Match::result eq (null as MatchResult?)), setValue(Match::result, MatchResult()))
    }
}