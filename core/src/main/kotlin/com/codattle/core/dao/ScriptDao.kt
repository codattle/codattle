package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Script
import com.codattle.core.model.ScriptBuilder
import org.bson.conversions.Bson
import org.litote.kmongo.`in`
import org.litote.kmongo.set
import org.litote.kmongo.setValue
import javax.inject.Singleton

@Singleton
class ScriptDao(private val daoUtils: DaoUtils) {

    fun getScript(scriptId: Id<Script>): Script {
        return daoUtils.get(scriptId)
                ?: throw IllegalArgumentException("Script with id \"$scriptId\" doesn't exist.")
    }

    fun getScripts(filter: Bson? = null): List<Script> {
        return daoUtils.getMany(filter)
    }

    fun saveScript(script: ScriptBuilder): Script {
        return daoUtils.save(script)
    }

    fun updateScriptCode(scriptId: Id<Script>, code: String) {
        daoUtils.findAndModify(scriptId, setValue(Script::code, code))
                ?: throw IllegalArgumentException("Script with id \"$scriptId\" doesn't exist.")
    }
}