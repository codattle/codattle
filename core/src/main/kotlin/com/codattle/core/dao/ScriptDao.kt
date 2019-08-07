package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Script
import org.litote.kmongo.`in`
import javax.inject.Singleton

@Singleton
class ScriptDao(private val daoUtils: DaoUtils) {

    fun getScripts(): List<Script> {
        return daoUtils.getMany(Script::class.java)
    }

    fun getScripts(ids: List<Id<Script>>): List<Script> {
        return daoUtils.getMany(Script::class.java, Script::id.`in`(ids))
    }

    fun createScript(script: Script.Builder): Script {
        return daoUtils.save(script)
    }

}