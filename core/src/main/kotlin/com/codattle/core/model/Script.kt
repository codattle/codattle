package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import com.codattle.core.dao.Id

data class Script(
        override var id: Id<Script>? = null,
        val game: Id<Game>,
        val code: String
) : DaoModel<Script>

