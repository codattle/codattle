package com.codattle.core.service

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.model.Script
import com.codattle.core.test.BaseTest
import com.codattle.core.test.DatabasePopulator
import com.codattle.core.test.QueueHelper
import com.codattle.core.test.TestUtils
import org.assertj.core.api.Assertions.assertThat
import org.assertj.core.api.Assertions.assertThatThrownBy
import org.junit.jupiter.api.Test
import javax.inject.Inject

class MatchServiceTest : BaseTest() {

    @Inject
    private lateinit var matchService: MatchService

    @Inject
    private lateinit var dao: DaoUtils

    @Test
    fun `get match if exists`() {
        val createdMatch = databasePopulator.createMatch()

        val fetchedMatch = matchService.getMatch(createdMatch.id)

        assertThat(fetchedMatch).isEqualTo(createdMatch)
    }

    @Test
    fun `throw exception if try to get nonexistent match`() {
        assertThatThrownBy { matchService.getMatch(TestUtils.nonexistentMatchId) }
                .isInstanceOf(IllegalArgumentException::class.java)
                .hasMessage("Match with id \"${TestUtils.nonexistentMatchId}\" doesn't exist.")
    }

    @Test
    fun `get matches of existent game`() {
        val gameId = databasePopulator.createGame().id
        val existentMatches = databasePopulator.createMatches(game = gameId)

        val fetchedMatches = matchService.getMatchesOfGame(gameId)

        assertThat(fetchedMatches).isEqualTo(existentMatches)
    }


    @Test
    fun `get matches of nonexistent game`() {
        val fetchedMatches = matchService.getMatchesOfGame(TestUtils.nonexistentGameId)

        assertThat(fetchedMatches).isEmpty()
    }

    @Test
    fun `get result of existent match`() {
        val resultOfExistentMatch = DatabasePopulator.generateMatchResult(framesCount = 10, winner = 0)
        val matchId = databasePopulator.createMatch(scriptsCount = 3, result = resultOfExistentMatch).id

        val fetchedMatchResult = matchService.getResultOfMatch(matchId)

        assertThat(fetchedMatchResult).isEqualTo(resultOfExistentMatch)
    }


    @Test
    fun `throw exception if try to get result of nonexistent match`() {
        assertThatThrownBy { matchService.getResultOfMatch(TestUtils.nonexistentMatchId) }
                .isInstanceOf(IllegalArgumentException::class.java)
                .hasMessage("Match with id \"${TestUtils.nonexistentMatchId}\" doesn't exist.")
    }

    @Test
    fun `create match`() {
        val gameId = databasePopulator.createGame().id

        val createdMatch = matchService.createMatch(name = "My match", gameId = gameId)

        assertThat(createdMatch.name).isEqualTo("My match")
        assertThat(createdMatch.game).isEqualTo(gameId)
        assertThat(createdMatch.scripts).isEqualTo(listOf<Script>())
        assertThat(createdMatch.result).isEqualTo(null)
    }

    @Test
    fun `add script to match after joining to not full match`() {
        val match = databasePopulator.createMatch(scriptsCount = 0, result = null)
        val scriptId = databasePopulator.createScript(game = match.game).id

        matchService.joinMatch(match.id, scriptId)

        val matchAfterJoining = dao.get(match.id)!!
        assertThat(matchAfterJoining.scripts).isEqualTo(listOf(scriptId))
    }

    @Test
    fun `send message to queue after joining twice to empty match`() {
        val match = databasePopulator.createMatch(scriptsCount = 0, result = null)
        val scripts = databasePopulator.createScripts(count = 2, game = match.game)

        scripts.forEach { matchService.joinMatch(match.id, it.id) }

        val matchAfterJoining = dao.get(match.id)!!
        assertThat(matchAfterJoining.scripts).isEqualTo(scripts.map { it.id })
        assertThat(queueHelper.countMessages(QueueHelper.SIMULATION)).isEqualTo(1)
        assertThat(queueHelper.readMessage(QueueHelper.SIMULATION)).isEqualTo(match.id.toString().toByteArray())
    }

    @Test
    fun `throw exception if try to join nonexistent match`() {
        val scriptId = databasePopulator.createScript().id

        assertThatThrownBy { matchService.joinMatch(TestUtils.nonexistentMatchId, scriptId) }
                .isInstanceOf(IllegalArgumentException::class.java)
                .hasMessage("Match with id \"${TestUtils.nonexistentMatchId}\" doesn't exist.")
    }

    @Test
    fun `throw exception if try to join full match`() {
        val scriptId = databasePopulator.createScript().id
        val matchId = databasePopulator.createMatch(scriptsCount = 2, result = null).id

        assertThatThrownBy { matchService.joinMatch(matchId, scriptId) }
                .isInstanceOf(IllegalArgumentException::class.java)
                .hasMessage("Cannot join to match with id $matchId")
    }
}
