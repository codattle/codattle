package com.codattle.core.resolver

import com.codattle.core.model.*
import com.codattle.core.service.FileService
import com.codattle.core.service.GameService
import com.codattle.core.service.ScriptService
import com.coxautodev.graphql.tools.GraphQLResolver
import javax.inject.Singleton

@Singleton
class SpriteResolver(private val fileService: FileService) : GraphQLResolver<Sprite> {

    fun image(sprite: Sprite): File? {
        return fileService.getFile(sprite.image)
    }
}