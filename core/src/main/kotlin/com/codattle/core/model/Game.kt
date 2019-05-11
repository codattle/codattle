package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import dev.morphia.annotations.Entity
import org.bson.types.ObjectId

@Entity(noClassnameStored = true)
class Game : DaoModel() {

    companion object {
        fun ref(id: ObjectId): Game {
            val game = Game()
            game.id = id
            return game
        }
    }

    var name: String? = null

    var mainLoop: String? = null

}