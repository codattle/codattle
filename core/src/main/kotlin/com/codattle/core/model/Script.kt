package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import dev.morphia.annotations.Entity
import org.bson.types.ObjectId

@Entity(noClassnameStored = true)
class Script : DaoModel() {

    companion object {
        fun ref(id: ObjectId): Script {
            val script = Script()
            script.id = id
            return script
        }
    }

    var content: String? = null

    var game: Game? = null

}