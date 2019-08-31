package com.codattle.core.resolver

import com.codattle.core.model.*
import com.codattle.core.service.GameService
import com.codattle.core.service.ScriptService
import com.codattle.core.service.UserService
import com.coxautodev.graphql.tools.GraphQLResolver
import javax.inject.Singleton

@Singleton
class FileResolver(private val userService: UserService) : GraphQLResolver<File> {

    fun owner(file: File): User {
        return userService.getUser(file.owner)
    }
}