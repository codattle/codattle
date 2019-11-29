package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import org.litote.kmongo.*
import javax.inject.Singleton

@Singleton
class MatchDao(private val daoUtils: DaoUtils) {

    fun getMatch(matchId: Id<Match>): Match {
        return daoUtils.get(matchId)
                ?: throw IllegalArgumentException("Match with id \"$matchId\" doesn't exist.")
    }

    fun getMatches(matchIds: List<Id<Match>>): List<Match> {
        return daoUtils.getMany(Match::id `in` matchIds)
    }

    fun getMatchesOfGame(gameId: Id<Game>): List<Match> {
        return daoUtils.getMany(Match::game eq gameId)
    }

    fun getResultOfMatch(matchId: Id<Match>): MatchResult? {
        return daoUtils.getWithFields(matchId, Match::class.java, Match::result)
                ?: throw IllegalArgumentException("Match with id \"$matchId\" doesn't exist.")
    }

    fun saveMatch(match: MatchBuilder): Match {
        return daoUtils.save(match)
    }

    fun addScriptIfScriptCountEquals(matchId: Id<Match>, scriptId: Id<Script>, scriptCount: Int): Match? {
        return daoUtils.findAndModify(matchId, Match::scripts size scriptCount, push(Match::scripts, scriptId))
    }

    fun provideResultFrames(matchId: Id<Match>, resultFrames: List<ResultFrame>) {
        createEmptyMatchResultIfDoesNotExist(matchId)
        daoUtils.findAndModify(matchId, pushEach(Match::result / MatchResult::resultFrames, resultFrames))
    }

    fun provideMatchWinner(matchId: Id<Match>, winner: Int) {
        createEmptyMatchResultIfDoesNotExist(matchId)
        val match = daoUtils.findAndModify(
                Match::class.java,
                and(Match::id eq matchId, (Match::result / MatchResult::winner) eq (null as Int?)),
                setValue(Match::result / MatchResult::winner, winner))
        match ?: throw IllegalArgumentException("Match with id $matchId doesn't exist or has already winner")
    }

    private fun createEmptyMatchResultIfDoesNotExist(matchId: Id<Match>) {
        daoUtils.findAndModify(Match::class.java, and(Match::id eq matchId, Match::result eq (null as MatchResult?)), setValue(Match::result, MatchResult()))
    }
}