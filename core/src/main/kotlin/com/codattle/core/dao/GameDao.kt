package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.GameBuilder
import com.codattle.core.model.Sprite
import org.bson.conversions.Bson
import org.litote.kmongo.contains
import org.litote.kmongo.eq
import org.litote.kmongo.pullByFilter
import org.litote.kmongo.push
import javax.inject.Singleton

@Singleton
class GameDao(private val daoUtils: DaoUtils) {

    fun getGame(gameId: Id<Game>): Game {
        return daoUtils.get(gameId)
                ?: throw IllegalArgumentException("Game with id \"$gameId\" doesn't exist.")
    }

    fun getGames(filter: Bson? = null): List<Game> {
        return daoUtils.getMany(filter)
    }

    fun isPlayersCountAllowed(gameId: Id<Game>, playersCount: Int): Boolean {
        return daoUtils.exists(gameId, Game::allowedPlayerCounts contains playersCount)
    }

    fun saveGame(game: GameBuilder): Game {
        return daoUtils.save(game)
    }

    fun addSprite(gameId: Id<Game>, sprite: Sprite) {
        daoUtils.findAndModify(gameId, push(Game::sprites, sprite))
    }

    fun removeSprite(gameId: Id<Game>, spriteName: String) {
        daoUtils.findAndModify(gameId, pullByFilter(Game::sprites, Sprite::name eq spriteName));
    }
}