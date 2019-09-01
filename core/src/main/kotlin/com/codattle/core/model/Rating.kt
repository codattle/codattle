package com.codattle.core.model

import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils
import java.time.Instant

data class Rating(
        override val id: Id<Rating>,
        override val creationDate: Instant,
        val author: Id<User>,
        val game: Id<Game>,
        val value: RatingValue,
        val description: String?
) : DaoModel<Rating> {

    data class Builder(
            override var id: Id<Rating>? = null,
            override var creationDate: Instant? = null,
            var author: Id<User>? = null,
            var game: Id<Game>? = null,
            var value: RatingValue? = null,
            var description: String? = null
    ) : DaoModelBuilder<Rating> {

        override fun build(): Rating {
            return Rating(
                    id = ModelUtils.nonNull(id, "id"),
                    creationDate = ModelUtils.nonNull(creationDate, "creationDate"),
                    author = ModelUtils.nonNull(author, "author"),
                    game = ModelUtils.nonNull(game, "game"),
                    value = ModelUtils.nonNull(value, "value"),
                    description = description
            )
        }
    }

    override fun toBuilder(): Builder {
        return Builder(
                id = id,
                creationDate = creationDate,
                author = author,
                game = game,
                value = value,
                description = description
        )
    }
}