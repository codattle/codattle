package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import com.codattle.core.dao.Id

data class Game(
        override var id: Id<Game>? = null,
        val name: String,
        val description: String,
        val code: String
) : DaoModel<Game>