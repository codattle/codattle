package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import dev.morphia.annotations.Entity

@Entity(noClassnameStored = true)
class Match : DaoModel() {

    var name: String? = null

    var game: Game? = null

    var scripts: List<Script> = listOf()

    var result: Result? = null

}