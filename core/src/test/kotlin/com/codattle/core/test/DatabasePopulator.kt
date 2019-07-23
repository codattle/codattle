package com.codattle.core.test

import com.codattle.core.dao.Dao
import com.codattle.core.dao.Id
import com.codattle.core.dao.MongoProvider
import com.codattle.core.model.*
import org.bson.Document
import javax.inject.Singleton

@Singleton
class DatabasePopulator(private val dao: Dao) {

    companion object {
        fun generateMatchResult(framesCount: Int = 3, winner: Int? = null): MatchResult {
            val resultFrames = (1..framesCount).map { ResultFrame(it - 1, content = "{epoch: $it}") }
            return MatchResult(resultFrames, winner)
        }
    }

    fun clean() {
        dao.getCollection(Game::class.java).deleteMany(Document())
        dao.getCollection(Match::class.java).deleteMany(Document())
        dao.getCollection(Script::class.java).deleteMany(Document())
    }

    fun createGame(name: String = "Test game", description: String = "It's test game", code: String = "end();"): Game {
        val game = Game(name = name, description = description, code = code)
        return dao.save(game)
    }

    fun createGames(count: Int = 3): List<Game> {
        return (1..count).map { createGame(name = "Game-$it", description = "It's Game No. $it", code = "end($it);") }
    }

    fun createScript(code: String = "answer(0);", game: Id<Game> = createGame().id!!): Script {
        val script = Script(code = code, game = game)
        return dao.save(script)
    }

    fun createScripts(count: Int = 3, game: Id<Game> = createGame().id!!): List<Script> {
        return (1..count).map { createScript(code = "answer($it);", game = game) }
    }

    fun createMatch(
            name: String = "Test match",
            game: Id<Game> = createGame().id!!,
            scriptsCount: Int = 2,
            result: MatchResult? = generateMatchResult(framesCount = 5, winner = 0)
    ): Match {
        val scripts = (1..scriptsCount).map { createScript(game = game).id!! }
        val match = Match(name = name, game = game, scripts = scripts, result = result)
        return dao.save(match)
    }

    fun createMatches(count: Int = 3, game: Id<Game> = createGame().id!!): List<Match> {
        return (1..count).map { createMatch(name = "Match-$it", game = game) }
    }
}