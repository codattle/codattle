package com.codattle.core.service

import com.codattle.core.model.Language
import com.codattle.core.model.LanguageMap
import com.codattle.core.test.BaseTest
import com.codattle.core.test.TestUtils
import org.assertj.core.api.Assertions.assertThat
import org.assertj.core.api.Assertions.assertThatThrownBy
import org.junit.jupiter.api.Test
import javax.inject.Inject

class GameServiceTest : BaseTest() {

    @Inject
    private lateinit var gameService: GameService

    @Test
    fun `get game if exists`() {
        val existentGame = databasePopulator.createGame()

        val fetchedGame = gameService.getGame(existentGame.id)

        assertThat(fetchedGame).isEqualTo(existentGame)
    }

    @Test
    fun `throw exception if try to get nonexistent game`() {
        assertThatThrownBy { gameService.getGame(TestUtils.nonexistentGameId) }
                .isInstanceOf(IllegalArgumentException::class.java)
                .hasMessage("Game with id \"${TestUtils.nonexistentGameId}\" doesn't exist.")
    }

    @Test
    fun `get all games`() {
        val existentGames = databasePopulator.createGames()

        val fetchedGames = gameService.getGames()

        assertThat(fetchedGames).isEqualTo(existentGames)
    }

    @Test
    fun `create game`() {
        val createdGame = gameService.createGame(name = "The Best Game", description = "It's the best game", code = "end(0);")



        assertThat(createdGame.name).isEqualTo("The Best Game")
        assertThat(createdGame.description).isEqualTo(listOf(LanguageMap(Language.EN, "It's the best game")))
        assertThat(createdGame.code).isEqualTo("end(0);")
    }
}
