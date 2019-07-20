package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import com.codattle.core.dao.Id

data class Match(
        override var id: Id<Match>? = null,
        val name: String,
        val game: Id<Game>,
        val scripts: List<Id<Script>> = listOf(),
        val result: MatchResult? = null
) : DaoModel<Match>