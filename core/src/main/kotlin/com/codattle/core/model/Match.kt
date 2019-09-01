package com.codattle.core.model

import com.codattle.annotation.processing.annotation.GenerateDaoModelBuilder
import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils.nonNull
import java.time.Instant

@GenerateDaoModelBuilder
data class Match(
        override val id: Id<Match>,
        override val creationDate: Instant,
        val name: String,
        val game: Id<Game>,
        val scripts: List<Id<Script>>,
        val result: MatchResult?
) : DaoModel<Match> {

    companion object {
        fun builder(): MatchBuilder {
            return MatchBuilder.withoutDefault(scripts = listOf())
        }
    }

    override fun toBuilder(): MatchBuilder {
        return MatchBuilder.fromModel(this)
    }
}