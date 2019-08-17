package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Sprite
import org.litote.kmongo.push
import javax.inject.Singleton

@Singleton
class GameDao(private val daoUtils: DaoUtils) {

    fun getGame(gameId: Id<Game>): Game {
        return daoUtils.get(gameId, Game::class.java)
                ?: throw IllegalArgumentException("Game with id \"$gameId\" doesn't exist.")
    }

    fun getGames(): List<Game> {
        return daoUtils.getMany(Game::class.java)
    }

    fun createGame(game: Game.Builder): Game {
        return daoUtils.save(game)
    }

    fun addSprite(gameId: Id<Game>, sprite: Sprite) {
        daoUtils.findAndModify(Game::class.java, gameId, push(Game::sprites, sprite))
    }
}