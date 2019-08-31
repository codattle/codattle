package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Rating
import org.litote.kmongo.and
import org.litote.kmongo.eq
import javax.inject.Singleton

@Singleton
class RatingDao(private val daoUtils: DaoUtils) {

    fun insertOrReplaceRating(rating: Rating.Builder): Rating {
        return daoUtils.insertOrReplace(rating, and(Rating::author eq rating.author, Rating::game eq rating.game))
    }

    fun getRatingsOfGame(gameId: Id<Game>): List<Rating> {
        return daoUtils.getMany(Rating::game eq gameId)
    }
}
