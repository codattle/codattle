package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.dao.Id
import com.codattle.core.model.*
import com.fasterxml.jackson.databind.ObjectMapper
import org.bson.types.ObjectId
import org.litote.kmongo.eq
import java.nio.ByteBuffer
import javax.inject.Singleton

@Singleton
class MatchService(private val dao: Dao, private val queueService: QueueService, private val objectMapper: ObjectMapper) {

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
        return dao.getMany(Match::class.java, Match::game.eq(gameId))
    }

    private data class MatchProjection(val result: Result?)

    fun getResultOfMatch(matchId: Id<Match>): Result? {
        val match = dao.getWithFields(matchId, Match::class.java, MatchProjection::class.java)
                ?: throw IllegalArgumentException("Match with id \"$matchId\" doesn't exist.")
        return match.result
    }

    fun createMatch(name: String, gameId: Id<Game>): Match {
        return dao.save(Match(name, gameId, listOf()))
    }

    fun joinMatch(matchId: Id<Match>, scriptId: Id<Script>) {
        val match = getMatch(matchId)
        if (!canJoinMatch(match)) {
            throw IllegalArgumentException("Cannot join to match with id $matchId")
        }
        match.scripts += scriptId
        dao.save(match)

        if (match.scripts.size == SCRIPTS_PER_MATCH) {
            startMatch(matchId)
        }
    }

    private fun canJoinMatch(match: Match): Boolean {
        return match.scripts.size < SCRIPTS_PER_MATCH
    }

    private fun startMatch(matchId: Id<Match>) {
        queueService.sendMessage(SIMULATION_QUEUE, ByteBuffer.allocate(Int.SIZE_BYTES).putInt(matchId.id).array())
    }

    // TODO: operation should be atomic
    fun provideResultFrames(matchId: Id<Match>, resultFrames: List<ResultFrame>) {
        val match = getMatch(matchId)
        match.result = match.result ?: Result()
        match.result!!.resultFrames += resultFrames
        dao.save(match)
    }

    // TODO: operation should be atomic
    fun provideMatchWinner(matchId: Id<Match>, winner: Int) {
        val match = getMatch(matchId)
        when {
            match.result == null -> match.result = Result(listOf(), winner)
            match.result!!.winner == null -> match.result!!.winner = winner
            else -> throw IllegalArgumentException("Match with id $matchId has already winner")
        }
        dao.save(match)
    }
}