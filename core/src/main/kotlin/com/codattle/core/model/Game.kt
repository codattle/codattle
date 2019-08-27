package com.codattle.core.model

import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils.nonNull
import java.time.Instant

data class Game(
        override val id: Id<Game>,
        override val creationDate: Instant,
        override val modificationDate: Instant,
        val name: String,
        val description: List<LanguageMap>,
        val code: String,
        val scriptTemplate: String,
        val rating: Map<Id<User>, GameOpinion>,
        val comments: Map<Id<User>, String>,
        val logo: Id<File>?,
        val author: Id<User>,
        val visibility: Visibility,
        val sprites: List<Sprite>
) : DaoModel<Game> {

    enum class Visibility {
        PUBLIC, PRIVATE
    }

    data class Builder(
            override var id: Id<Game>? = null,
            override var creationDate: Instant? = null,
            override var modificationDate: Instant? = null,
            val name: String? = null,
            val description: List<LanguageMap> = listOf(),
            val code: String? = null,
            val scriptTemplate: String = "",
            val rating: Map<Id<User>, GameOpinion> = mapOf(),
            val comments: Map<Id<User>, String> = mapOf(),
            val logo: Id<File>? = null,
            val author: Id<User>? = null,
            val visibility: Visibility = Visibility.PRIVATE,
            val sprites: List<Sprite> = listOf()
    ) : DaoModelBuilder<Game> {

        override fun build(): Game {
            return Game(
                    id = nonNull(id, "id"),
                    creationDate = nonNull(creationDate, "creationDate"),
                    modificationDate = nonNull(modificationDate, "modificationDate"),
                    name = nonNull(name, "name"),
                    description = description,
                    code = nonNull(code, "code"),
                    scriptTemplate = scriptTemplate,
                    rating = rating,
                    comments = comments,
                    logo = logo,
                    author = nonNull(author, "author"),
                    visibility = visibility,
                    sprites = sprites
            )
        }

        override fun getModelClass(): Class<Game> {
            return Game::class.java
        }
    }

    override fun toBuilder(): Builder {
        return Builder(
                id = id,
                creationDate = creationDate,
                modificationDate = modificationDate,
                name = name,
                description = description,
                code = code,
                scriptTemplate = scriptTemplate,
                rating = rating,
                comments = comments,
                logo = logo,
                author = author,
                visibility = visibility,
                sprites = sprites
        )
    }
}