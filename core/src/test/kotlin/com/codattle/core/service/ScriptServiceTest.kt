package com.codattle.core.service

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.Script
import com.codattle.core.test.BaseTest
import com.codattle.core.test.TestUtils
import org.assertj.core.api.Assertions.assertThat
import org.assertj.core.api.Assertions.assertThatThrownBy
import javax.inject.Inject

class ScriptServiceTest : BaseTest() {

    @Inject
    private lateinit var scriptService: ScriptService

    @Inject
    private lateinit var dao: DaoUtils

    fun `get scripts if all exists`() {
        val game = databasePopulator.createGame()
        val scripts = databasePopulator.createScripts(count = 4, game = game.id).map { it.id }

        val fetchedScripts = scriptService.getScripts(scripts)

        assertThat(fetchedScripts).hasSize(4)
    }

    fun `get scripts with the same id`() {
        val game = databasePopulator.createGame()
        val script = databasePopulator.createScript(game = game.id).id

        val scripts = scriptService.getScripts(listOf(script, script))

        assertThat(scripts).hasSize(2)
        assertThat(scripts[0]).isSameAs(scripts[1])
    }

    fun `throw exeption if at least one script doesn't exist`() {
        val game = databasePopulator.createGame()
        val script = databasePopulator.createScript(game = game.id).id

        val scriptIds = listOf(script, TestUtils.nonexistentScriptId)
        assertThatThrownBy { scriptService.getScripts(scriptIds) }
                .isInstanceOf(IllegalArgumentException::class.java)
                .hasMessage("Script with id \"${TestUtils.nonexistentScriptId}\" doesn't exist.")
    }
}