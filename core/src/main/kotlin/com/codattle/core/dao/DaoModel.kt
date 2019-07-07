package com.codattle.core.dao

import org.bson.codecs.pojo.annotations.BsonId
import org.bson.types.ObjectId

abstract class DaoModel<T: DaoModel<T>> {

    @BsonId var internalId: ObjectId? = null
    var id: Id<T>? = null
}