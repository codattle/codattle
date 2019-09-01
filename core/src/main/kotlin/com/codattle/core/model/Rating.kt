package com.codattle.core.model

import com.codattle.annotation.processing.annotation.GenerateDaoModelBuilder
import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.Id
import java.time.Instant

@GenerateDaoModelBuilder
data class Rating(
        override val id: Id<Rating>,
        override val creationDate: Instant,
        val author: Id<User>,
        val game: Id<Game>,
        val value: RatingValue,
        val description: String?
) : DaoModel<Rating> {

    companion object {
        fun builder(): RatingBuilder {
            return RatingBuilder.withoutDefault()
        }
    }

    override fun toBuilder(): RatingBuilder {
        return RatingBuilder.fromModel(this)
    }
}