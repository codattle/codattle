package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.model.Game
import org.bson.types.ObjectId
import javax.inject.Singleton

@Singleton
class GameService(private val dao: Dao) {

    fun getGame(gameId: ObjectId): Game {
        return dao.get(gameId, Game::class.java)
                ?: throw IllegalArgumentException("Game with id \"$gameId\" doesn't exist.")
    }

    fun getGames(): List<Game> {
        return dao.getAll(Game::class.java)
    }

    fun createGame(name: String, mainLoop: String): Game {
        val game = Game()
        game.name = name
        game.mainLoop = mainLoop
        return dao.saveAndGet(game)
    }

}