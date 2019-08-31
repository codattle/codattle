package com.codattle.core.resolver

import com.codattle.core.model.*
import com.codattle.core.service.FileService
import com.codattle.core.service.RatingService
import com.coxautodev.graphql.tools.GraphQLResolver
import javax.inject.Singleton

@Singleton
class GameResolver(private val fileService: FileService, private val ratingService: RatingService) : GraphQLResolver<Game> {

    fun description(game: Game): List<TranslatedText> {
        return game.description.toListOfTranslatedTexts()
    }

    fun logo(game: Game): File? {
        return game.logo?.let { fileService.getFile(it) }
    }

    fun ratings(game: Game): List<Rating> {
        return ratingService.getRatingsOfGame(game.id)
    }
}