package com.codattle.core.service

import com.codattle.core.dao.ScriptDao
import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Script
import org.litote.kmongo.`in`
import javax.inject.Singleton

@Singleton
class ScriptService(private val scriptDao: ScriptDao) {

    fun getScripts(): List<Script> {
        return scriptDao.getScripts()
    }

    fun getScripts(ids: List<Id<Script>>): List<Script> {
        return scriptDao.getScripts(ids)
    }

    fun createScript(gameId: Id<Game>, code: String): Script {
        return scriptDao.createScript(Script.Builder(
                game = gameId,
                code = code,
                // TODO: pass real user after implementing users
                author = Id("nonexistent_user")
        ))
    }
}