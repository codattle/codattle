package com.codattle.core.service

import com.codattle.core.dao.RatingDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Rating
import javax.inject.Singleton

@Singleton
class RatingService(private val ratingDao: RatingDao) {

    fun getRatingsOfGame(gameId: Id<Game>): List<Rating> {
        return ratingDao.getRatingsOfGame(gameId)
    }
}