package com.codattle.core.model

import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils.nonNull
import java.time.Instant

data class Match(
        override val id: Id<Match>,
        override val creationDate: Instant,
        val name: String,
        val game: Id<Game>,
        val scripts: List<Id<Script>>,
        val result: MatchResult?
) : DaoModel<Match> {

    data class Builder(
            override var id: Id<Match>? = null,
            override var creationDate: Instant? = null,
            var name: String? = null,
            var game: Id<Game>? = null,
            var scripts: List<Id<Script>> = listOf(),
            var result: MatchResult? = null
    ) : DaoModelBuilder<Match> {

        override fun build(): Match {
            return Match(
                    id = nonNull(id, "id"),
                    creationDate = nonNull(creationDate, "creationDate"),
                    name = nonNull(name, "name"),
                    game = nonNull(game, "game"),
                    scripts = scripts,
                    result = result
            )
        }
    }

    override fun toBuilder(): Builder {
        return Builder(
                id = id,
                creationDate = creationDate,
                name = name,
                game = game,
                scripts = scripts,
                result = result
        )
    }
}