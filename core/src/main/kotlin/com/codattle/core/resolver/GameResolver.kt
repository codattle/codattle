package com.codattle.core.resolver

import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import com.codattle.core.service.FileService
import com.codattle.core.service.RatingService
import com.codattle.core.service.ScriptService
import com.coxautodev.graphql.tools.GraphQLResolver
import javax.inject.Singleton

@Singleton
class GameResolver(
        private val fileService: FileService,
        private val ratingService: RatingService,
        private val scriptService: ScriptService
) : GraphQLResolver<Game> {

    fun description(game: Game): List<TranslatedText> {
        return game.description.toListOfTranslatedTexts()
    }

    fun logo(game: Game): File? {
        return game.logo?.let { fileService.getFile(it) }
    }

    fun ratings(game: Game): List<Rating> {
        return ratingService.getRatingsOfGame(game.id)
    }

    fun scripts(game: Game, authorId: Id<User>?): List<Script> {
        return scriptService.getScripts(game.id, authorId)
    }
}