package com.codattle.core.model

import com.codattle.annotation.processing.annotation.GenerateDaoModelBuilder
import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.Id
import java.time.Instant

@GenerateDaoModelBuilder
data class Tournament(
        override val id: Id<Tournament>,
        override val creationDate: Instant,
        val name: String,
        val game: Id<Game>,
        val scripts: List<Id<Script>>,
        val maxScriptCount: Int,
        val matches: List<Id<Match>>?
) : DaoModel<Tournament> {

    companion object {
        fun builder(): TournamentBuilder {
            return TournamentBuilder.withoutDefault(scripts = listOf())
        }
    }

    override fun toBuilder(): TournamentBuilder {
        return TournamentBuilder.fromModel(this)
    }
}