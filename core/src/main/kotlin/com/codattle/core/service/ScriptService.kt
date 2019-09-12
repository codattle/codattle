package com.codattle.core.service

import com.codattle.core.dao.ScriptDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Script
import com.codattle.core.model.User
import org.litote.kmongo.`in`
import org.litote.kmongo.and
import org.litote.kmongo.eq
import javax.inject.Singleton

@Singleton
class ScriptService(private val scriptDao: ScriptDao) {

    fun getScript(id: Id<Script>): Script {
        return scriptDao.getScript(id)
    }

    fun getScripts(ids: List<Id<Script>>): List<Script> {
        return if (ids.isEmpty()) {
            emptyList()
        } else {
            val scripts = scriptDao.getScripts(Script::id `in` ids)
            require(scripts.size == ids.size) { "At least one script doesn't exist: $ids" }
            scripts
        }
    }

    fun getScripts(gameId: Id<Game>? = null, authorId: Id<User>? = null): List<Script> {
        val filter = and(gameId?.let { Script::game eq it }, authorId?.let { Script::author eq it })
        return scriptDao.getScripts(filter)
    }

    fun createScript(gameId: Id<Game>, code: String, author: Id<User>): Script {
        return scriptDao.saveScript(Script.builder()
                .game(gameId)
                .code(code)
                .author(author)
        )
    }

    fun updateScriptCode(scriptId: Id<Script>, code: String) {
        scriptDao.updateScriptCode(scriptId, code)
    }
}