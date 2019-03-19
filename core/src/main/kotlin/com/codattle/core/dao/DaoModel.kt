package com.codattle.core.dao

import dev.morphia.annotations.Id
import org.bson.types.ObjectId

open class DaoModel {

    @Id
    var id: ObjectId? = null

}