package com.codattle.core.test

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.*
import org.bson.Document
import java.util.*
import javax.inject.Singleton

@Singleton
class DatabasePopulator(private val daoUtils: DaoUtils) {

    companion object {
        fun generateMatchResult(framesCount: Int = 3, winner: Int? = null): MatchResult {
            val resultFrames = (1..framesCount).map { ResultFrame(it - 1, content = "{epoch: $it}") }
            return MatchResult(resultFrames, winner)
        }
    }

    fun clean() {
        daoUtils.getCollection(Game::class.java).deleteMany(Document())
        daoUtils.getCollection(Match::class.java).deleteMany(Document())
        daoUtils.getCollection(Script::class.java).deleteMany(Document())
    }

    fun createGame(name: String = "Test game", description: String = "It's test game", code: String = "end();"): Game {
        return daoUtils.save(Game.Builder(name = name, description = I18nText.single(Language.EN, description), code = code, author = createUser().id))
    }

    fun createGames(count: Int = 3): List<Game> {
        return (1..count).map { createGame(name = "Game-$it", description = "It's Game No. $it", code = "end($it);") }
    }

    fun createScript(code: String = "answer(0);", game: Id<Game> = createGame().id): Script {
        return daoUtils.save(Script.Builder(code = code, game = game, author = createUser().id))
    }

    fun createScripts(count: Int = 3, game: Id<Game> = createGame().id): List<Script> {
        return (1..count).map { createScript(code = "answer($it);", game = game) }
    }

    fun createMatch(
            name: String = "Test match",
            game: Id<Game> = createGame().id,
            scriptsCount: Int = 2,
            result: MatchResult? = generateMatchResult(framesCount = 5, winner = 0)
    ): Match {
        val scripts = (1..scriptsCount).map { createScript(game = game).id }
        return daoUtils.save(Match.Builder(name = name, game = game, scripts = scripts, result = result))
    }

    fun createMatches(count: Int = 3, game: Id<Game> = createGame().id): List<Match> {
        return (1..count).map { createMatch(name = "Match-$it", game = game) }
    }

    fun createUser(): User {
        return daoUtils.save(User.Builder(username = UUID.randomUUID().toString()))
    }
}