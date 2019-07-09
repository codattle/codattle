package com.codattle.core.model

import com.codattle.core.dao.DaoModel

data class Game(
        var name: String,
        var description: String,
        var code: String
) : DaoModel<Game>()