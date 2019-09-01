package com.codattle.core.service

import com.codattle.core.dao.ScriptDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Script
import com.codattle.core.model.User
import javax.inject.Singleton

@Singleton
class ScriptService(private val scriptDao: ScriptDao) {

    fun getScripts(): List<Script> {
        return scriptDao.getScripts()
    }

    fun getScripts(ids: List<Id<Script>>): List<Script> {
        return scriptDao.getScripts(ids)
    }

    fun createScript(gameId: Id<Game>, code: String, author: Id<User>): Script {
        return scriptDao.saveScript(Script.builder()
                .game(gameId)
                .code(code)
                .author(author)
        )
    }
}