package com.codattle.core.service

import com.codattle.core.dao.GameDao
import com.codattle.core.dao.RatingDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import org.litote.kmongo.regex
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

    fun searchGames(name: String? = null): List<Game> {
        val filter = name?.let { Game::name regex ("^" + Regex.escape(it)) }
        return gameDao.getGames(filter)
    }

    data class NewGame(
            val name: String,
            val description: String,
            val code: String,
            val author: Id<User>,
            val logo: Id<File>? = null,
            val sprites: List<Sprite> = listOf(),
            val allowedPlayerCounts: Set<Int>
    )

    fun createGame(game: NewGame): Game {
        return gameDao.saveGame(Game.builder()
                .name(game.name)
                .description(I18nText.single(DEFAULT_DESCRIPTION_LANGUAGE, game.description))
                .code(game.code)
                .logo(game.logo)
                .author(game.author)
                .sprites(game.sprites)
                .allowedPlayerCounts(game.allowedPlayerCounts)
        )
    }

    fun getSprite(gameId: Id<Game>, spriteName: String): Sprite? {
        return getGame(gameId).sprites.find { sprite -> sprite.name == spriteName }
    }

    fun addSprite(gameId: Id<Game>, sprite: Sprite)  {
        gameDao.addSprite(gameId, sprite)
    }

    fun removeSprite(gameId: Id<Game>, spriteName: String) {
        getSprite(gameId, spriteName)?.let { spriteToRemove -> fileService.deleteFile(spriteToRemove.image)  }
        gameDao.removeSprite(gameId, spriteName)
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