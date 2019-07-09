package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import com.codattle.core.dao.Id

data class Script(var game: Id<Game>, var code: String) : DaoModel<Script>()

