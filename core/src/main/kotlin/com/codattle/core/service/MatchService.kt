package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.model.*
import org.bson.types.ObjectId
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

    fun getMatches(gameId: ObjectId): List<Match> {
        return dao.getAllWithFieldEqual(Match::class.java, "game._id", gameId)
    }

    fun getResultOfMatch(matchId: ObjectId): Result {
        val match = dao.getWithFields(matchId, Match::class.java, listOf("result"))
        return match.result!!
    }

    fun createMatch(name: String, gameId: ObjectId): Match {
        val match = Match()
        match.name = name
        match.game = Game.ref(gameId)
        return dao.saveAndGet(match)
    }

    fun joinMatch(matchId: ObjectId, scriptId: ObjectId) {
        val match = dao.get(matchId, Match::class.java)
        if (!canJoinMatch(match)) {
            throw IllegalArgumentException("Cannot join to match with id $matchId")
        }
        match.scripts += Script.ref(scriptId)
        dao.save(match)

        if (match.scripts.size == SCRIPTS_PER_MATCH) {
            startMatch(matchId)
        }
    }

    private fun canJoinMatch(match: Match): Boolean {
        return match.scripts.size < SCRIPTS_PER_MATCH
    }

    private fun startMatch(matchId: ObjectId) {
        queueService.sendMessage(SIMULATION_QUEUE, matchId.toByteArray())
    }

    fun provideResultFrames(matchId: ObjectId, resultFrames: List<ResultFrame>) {
        val match = dao.get(matchId, Match::class.java)
        match.result!!.resultFrames += resultFrames
        dao.save(match)
    }

}