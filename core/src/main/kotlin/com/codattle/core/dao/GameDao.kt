package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.GameBuilder
import com.codattle.core.model.Sprite
import com.codattle.core.model.User
import com.mongodb.client.model.Updates
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
        daoUtils.findAndModify(Game::class.java, gameId, push(Game::sprites, sprite))
    }
}