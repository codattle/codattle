package com.codattle.core.dao

import dev.morphia.annotations.Id
import org.bson.types.ObjectId

abstract class DaoModel {

    @Id
    var id: ObjectId? = null

}