package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.dao.Id
import com.codattle.core.model.Game
import javax.inject.Singleton

@Singleton
class GameService(private val dao: Dao) {

    fun getGame(gameId: Id<Game>): Game {
        return dao.get(gameId, Game::class.java)
                ?: throw IllegalArgumentException("Game with id \"$gameId\" doesn't exist.")
    }

    fun getGames(): List<Game> {
        return dao.getMany(Game::class.java)
    }

    fun createGame(name: String, description: String, code: String): Game {
        val game = Game(name, description, code)
        return dao.save(game)
    }
}