package com.codattle.core.migration

import com.codattle.core.model.Game
import com.codattle.core.model.Sprite
import com.codattle.core.model.User
import com.github.cloudyrock.mongock.ChangeLog
import com.github.cloudyrock.mongock.ChangeSet
import com.mongodb.client.MongoDatabase
import org.litote.kmongo.Id
import org.litote.kmongo.div
import org.litote.kmongo.ensureUniqueIndex
import org.litote.kmongo.getCollection
import java.time.Instant

@ChangeLog
class M001_initialization {

    @ChangeSet(order = "001", id = "M001_initialization_add_indexes", author = "Piotr Walkusz")
    fun addIndexes(db: MongoDatabase) {
        db.getCollection<Game>().ensureUniqueIndex(Game::id, Game::sprites / Sprite::name)
    }

    @ChangeSet(order = "002", id = "M001_initialization_add_default_user", author = "Piotr Walkusz")
    fun addDefaultUser(db: MongoDatabase) {
        db.getCollection<User>().insertOne(User(id = User.DEFAULT_USER, creationDate = Instant.now(), username = "test"))
    }
}