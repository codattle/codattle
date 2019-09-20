package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.GameBuilder
import com.codattle.core.model.Sprite
import com.codattle.core.model.User
import com.mongodb.client.model.Updates
import org.bson.conversions.Bson
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

    fun getGames(): List<Game> {
        return daoUtils.getMany()
    }

    fun saveGame(game: GameBuilder): Game {
        return daoUtils.save(game)
    }

    fun addSprite(gameId: Id<Game>, sprite: Sprite) {
        modifyGame(gameId, push(Game::sprites, sprite))
    }

    fun removeSprite(gameId: Id<Game>, spriteName: String) {
        modifyGame(gameId, pullByFilter(Game::sprites, Sprite::name eq spriteName));
    }

    private fun modifyGame(gameId: Id<Game>, updater: Bson): Game? {
        return daoUtils.findAndModify(Game::class.java, gameId, updater);
    }

}