package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import com.codattle.core.dao.Id

data class Match(
        var name: String,
        var game: Id<Game>,
        var scripts: List<Id<Script>>,
        var result: Result? = null
) : DaoModel<Match>()