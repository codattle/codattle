package com.codattle.core.dto

import com.codattle.core.dao.common.Id
import com.codattle.core.model.Match
import com.codattle.core.model.MatchResult
import com.codattle.core.model.Script
import com.codattle.core.service.ScriptService

data class MatchWithScriptsDTO(val id: Id<Match>, val name: String, val scripts: List<Script>, val result: MatchResult?) {

    companion object {
        fun from(match: Match, scriptService: ScriptService): MatchWithScriptsDTO {
            return MatchWithScriptsDTO(match.id, match.name, scriptService.getScripts(match.scripts), match.result)
        }
    }
}