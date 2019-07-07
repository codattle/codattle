package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.dao.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Script
import org.litote.kmongo.`in`
import org.litote.kmongo.eq
import javax.inject.Singleton

@Singleton
class ScriptService(private val dao: Dao) {

    fun getScripts(): List<Script> {
        return dao.getMany(Script::class.java)
    }

    fun getScripts(ids: List<Id<Script>>): List<Script> {
        return dao.getMany(Script::class.java, Script::id.`in`(ids))
    }

    fun createScript(gameId: Id<Game>, code: String): Script {
        val script = Script(gameId, code)
        return dao.save(script)
    }

}