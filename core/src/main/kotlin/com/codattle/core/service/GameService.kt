package com.codattle.core.service

import com.codattle.core.dao.GameDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import javax.inject.Singleton

@Singleton
class GameService(private val gameDao: GameDao) {

    companion object {
        // TODO: remove after implementing translatable descriptions
        private val DEFAULT_DESCRIPTION_LANGUAGE = Language.ENGLISH
    }

    fun getGame(gameId: Id<Game>): Game {
        return gameDao.getGame(gameId)
    }

    fun getGames(): List<Game> {
        return gameDao.getGames()
    }

    fun createGame(name: String, description: String, code: String, logo: Id<File>? = null, sprites: List<Sprite> = listOf()): Game {
        return gameDao.createGame(Game.Builder(
                name = name,
                description = I18nText(DEFAULT_DESCRIPTION_LANGUAGE, description),
                code = code,
                logo = logo,
                // TODO: pass real user after implementing users
                author = Id("nonexistent_user"),
                sprites = sprites
        ))
    }

    fun addSprite(gameId: Id<Game>, sprite: Sprite) {
        gameDao.addSprite(gameId, sprite)
    }
}