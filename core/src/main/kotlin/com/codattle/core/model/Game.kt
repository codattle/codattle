package com.codattle.core.model

import com.codattle.annotation.processing.annotation.GenerateDaoModelBuilder
import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.Id
import java.time.Instant

@GenerateDaoModelBuilder
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

    companion object {
        fun builder(): GameBuilder {
            return GameBuilder.withoutDefault(
                    description = I18nText(),
                    scriptTemplate = "",
                    visibility = Visibility.PRIVATE,
                    sprites = listOf()
            )
        }
    }

    enum class Visibility {
        PUBLIC, PRIVATE
    }

    override fun toBuilder(): GameBuilder {
        return GameBuilder.fromModel(this)
    }
}