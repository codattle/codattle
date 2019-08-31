package com.codattle.core.model

import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils.nonNull
import java.time.Instant

data class Game(
        override val id: Id<Game>,
        override val creationDate: Instant,
        val name: String,
        val description: I18nText,
        val code: String,
        val scriptTemplate: String,
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
            val name: String? = null,
            val description: I18nText = I18nText(),
            val code: String? = null,
            val scriptTemplate: String = "",
            val logo: Id<File>? = null,
            val author: Id<User>? = null,
            val visibility: Visibility = Visibility.PRIVATE,
            val sprites: List<Sprite> = listOf()
    ) : DaoModelBuilder<Game> {

        override fun build(): Game {
            return Game(
                    id = nonNull(id, "id"),
                    creationDate = nonNull(creationDate, "creationDate"),
                    name = nonNull(name, "name"),
                    description = description,
                    code = nonNull(code, "code"),
                    scriptTemplate = scriptTemplate,
                    logo = logo,
                    author = nonNull(author, "author"),
                    visibility = visibility,
                    sprites = sprites
            )
        }
    }

    override fun toBuilder(): Builder {
        return Builder(
                id = id,
                creationDate = creationDate,
                name = name,
                description = description,
                code = code,
                scriptTemplate = scriptTemplate,
                logo = logo,
                author = author,
                visibility = visibility,
                sprites = sprites
        )
    }
}