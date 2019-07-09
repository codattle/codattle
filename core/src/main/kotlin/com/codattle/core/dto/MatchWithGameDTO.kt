package com.codattle.core.dto

import com.codattle.core.dao.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Match
import com.codattle.core.model.Script
import com.codattle.core.model.Result
import org.bson.types.ObjectId

data class MatchWithGameDTO(
        val id: Id<Match>,
        val name: String,
        val scripts: List<Script>,
        val result: Result?,
        val game: Game
)