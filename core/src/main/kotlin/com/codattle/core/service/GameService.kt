package com.codattle.core.service

import com.codattle.core.dao.GameDao
import com.codattle.core.dao.RatingDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import org.litote.kmongo.regex
import javax.inject.Singleton

@Singleton
class GameService(private val gameDao: GameDao, private val ratingDao: RatingDao) {

    companion object {
        // TODO: remove after implementing translatable descriptions
        private val DEFAULT_DESCRIPTION_LANGUAGE = Language.EN
    }

    fun getGame(gameId: Id<Game>): Game {
        return gameDao.getGame(gameId)
    }

    fun getGames(): List<Game> {
        return gameDao.getGames()
    }

    fun searchGames(name: String? = null): List<Game> {
        val filter = name?.let { Game::name regex ("^" + Regex.escape(it)) }
        return gameDao.getGames(filter)
    }

    fun createGame(name: String, description: String, code: String, author: Id<User>, logo: Id<File>? = null, sprites: List<Sprite> = listOf()): Game {
        return gameDao.saveGame(Game.builder()
                .name(name)
                .description(I18nText.single(DEFAULT_DESCRIPTION_LANGUAGE, description))
                .code(code)
                .logo(logo)
                .author(author)
                .sprites(sprites)
        )
    }

    fun addSprite(gameId: Id<Game>, sprite: Sprite) {
        gameDao.addSprite(gameId, sprite)
    }

    fun rateGame(gameId: Id<Game>, userId: Id<User>, ratingValue: RatingValue, description: String?): Rating {
        return ratingDao.insertOrReplaceRating(Rating.builder()
                .author(userId)
                .game(gameId)
                .value(ratingValue)
                .description(description)
        )
    }
}