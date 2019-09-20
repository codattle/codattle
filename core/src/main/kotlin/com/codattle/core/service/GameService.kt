package com.codattle.core.service

import com.codattle.core.dao.GameDao
import com.codattle.core.dao.RatingDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import javax.inject.Singleton

@Singleton
class GameService(private val gameDao: GameDao, private val ratingDao: RatingDao, private val fileService: FileService) {

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

    fun addSprite(gameId: Id<Game>, sprite: Sprite)  {
        gameDao.addSprite(gameId, sprite)
    }

    fun removeSprite(gameId: Id<Game>, sprite: Sprite) {
        gameDao.removeSprite(gameId, sprite.name)
        fileService.deleteFile(sprite.image)
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