package com.codattle.core.model

import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils.nonNull
import java.time.Instant

data class Script(
        override val id: Id<Script>,
        override val creationDate: Instant,
        override val modificationDate: Instant,
        val game: Id<Game>,
        val code: String,
        val author: Id<User>,
        val visibility: Visibility
) : DaoModel<Script> {

    enum class Visibility {
        PUBLIC, PRIVATE
    }

    data class Builder(
            override var id: Id<Script>? = null,
            override var creationDate: Instant? = null,
            override var modificationDate: Instant? = null,
            var game: Id<Game>? = null,
            var code: String? = null,
            var author: Id<User>? = null,
            var visibility: Visibility = Visibility.PRIVATE
    ) : DaoModelBuilder<Script> {

        override fun build(): Script {
            return Script(
                    id = nonNull(id, "id"),
                    creationDate = nonNull(creationDate, "creationDate"),
                    modificationDate = nonNull(modificationDate, "modificationDate"),
                    game = nonNull(game, "game"),
                    code = nonNull(code, "code"),
                    author = nonNull(author, "author"),
                    visibility = visibility
            )
        }

        override fun getModelClass(): Class<Script> {
            return Script::class.java
        }
    }

    override fun toBuilder(): Builder {
        return Builder(
                id = id,
                creationDate = creationDate,
                modificationDate = modificationDate,
                game = game,
                code = code,
                author = author,
                visibility = visibility
        )
    }
}

