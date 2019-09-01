package com.codattle.core.resolver

import com.codattle.core.model.*
import com.codattle.core.service.GameService
import com.codattle.core.service.ScriptService
import com.codattle.core.service.UserService
import com.coxautodev.graphql.tools.GraphQLResolver
import javax.inject.Singleton

@Singleton
class RatingResolver(private val userService: UserService, private val gameService: GameService) : GraphQLResolver<Rating> {

    fun author(rating: Rating): User {
        return userService.getUser(rating.author)
    }

    fun game(rating: Rating): Game {
        return gameService.getGame(rating.game)
    }
}