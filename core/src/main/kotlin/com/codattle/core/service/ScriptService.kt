package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.model.Game
import com.codattle.core.model.Script
import org.bson.types.ObjectId
import javax.inject.Singleton

@Singleton
class ScriptService(private val dao: Dao) {

    fun getScripts(): List<Script> {
        return dao.getAll(Script::class.java)
    }

    fun createScript(gameId: ObjectId, content: String): Script {
        val script = Script()
        script.game = Game.ref(gameId)
        script.content = content
        return dao.saveAndGet(script)
    }

}