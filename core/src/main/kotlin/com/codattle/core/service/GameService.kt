package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.model.Game
import javax.inject.Singleton

@Singleton
class GameService(private val dao: Dao) {

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