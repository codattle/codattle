package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import org.litote.kmongo.eq
import org.litote.kmongo.push
import org.litote.kmongo.setValue
import org.litote.kmongo.size
import javax.inject.Singleton

@Singleton
class TournamentDao(private val daoUtils: DaoUtils) {

    fun getTournament(tournamentId: Id<Tournament>): Tournament {
        return daoUtils.get(tournamentId)
                ?: throw IllegalArgumentException("Tournament with id \"$tournamentId\" doesn't exist.")
    }

    fun getTournamentsOfGame(gameId: Id<Game>): List<Tournament> {
        return daoUtils.getMany(Tournament::game eq gameId)
    }

    fun saveTournament(tournament: TournamentBuilder): Tournament {
        return daoUtils.save(tournament)
    }

    fun addScriptIfScriptCountEquals(tournamentId: Id<Tournament>, scriptId: Id<Script>, scriptCount: Int): Tournament? {
        return daoUtils.findAndModify(tournamentId, Tournament::scripts size scriptCount, push(Tournament::scripts, scriptId))
    }

    fun setMatches(tournamentId: Id<Tournament>, matches: List<Id<Match>>): Tournament {
        return daoUtils.findAndModify(tournamentId, setValue(Tournament::matches, matches))
            ?: throw IllegalArgumentException("Tournament with id \"$tournamentId\" doesn't exist.")
    }
}