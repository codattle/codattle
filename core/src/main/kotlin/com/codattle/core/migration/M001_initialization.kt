package com.codattle.core.migration

import com.codattle.core.model.Game
import com.codattle.core.model.Sprite
import com.github.cloudyrock.mongock.ChangeLog
import com.github.cloudyrock.mongock.ChangeSet
import com.mongodb.client.MongoDatabase
import org.litote.kmongo.div
import org.litote.kmongo.ensureUniqueIndex
import org.litote.kmongo.getCollection

@ChangeLog
class M001_initialization {

    @ChangeSet(order = "001", id = "M001_initialization_add_indexes", author = "Piotr Walkusz")
    fun addIndexes(db: MongoDatabase) {
        db.getCollection<Game>().ensureUniqueIndex(Game::id, Game::sprites / Sprite::name)
    }
}