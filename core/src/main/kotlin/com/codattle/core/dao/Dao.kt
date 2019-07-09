package com.codattle.core.dao

import com.fasterxml.jackson.databind.ObjectMapper
import com.mongodb.client.MongoCollection
import org.bson.conversions.Bson
import org.litote.kmongo.eq
import org.litote.kmongo.findOne
import org.litote.kmongo.projection
import org.litote.kmongo.util.KMongoUtil
import javax.inject.Singleton
import kotlin.reflect.full.memberProperties

@Singleton
class Dao(
        private val mongoProvider: MongoProvider,
        private val sequence: Sequence,
        private val objectMapper: ObjectMapper
) {

    fun <T : DaoModel<T>> save(entity: T): T {
        if (entity.id == null) {
            entity.id = Id(sequence.getNext(KMongoUtil.defaultCollectionName(entity.javaClass.kotlin)))
            getCollection(entity.javaClass).insertOne(entity)
        } else {
            getCollection(entity.javaClass).replaceOne(DaoModel<T>::id.eq(entity.id), entity)
        }
        return entity
    }

    fun <T : DaoModel<T>> get(id: Id<T>, model: Class<T>): T? {
        return getCollection(model).findOne(DaoModel<T>::id.eq(id))
    }

    fun <T : DaoModel<T>, R> getWithFields(id: Id<T>, model: Class<T>, projection: Class<R>): R? {
        val document = mongoProvider.database.getCollection(KMongoUtil.defaultCollectionName(model.kotlin))
                .find(DaoModel<T>::id.eq(id))
                .projection(*projection.javaClass.kotlin.memberProperties.toTypedArray())
                .first()
        return if (document != null) objectMapper.readValue(document.toJson(), projection) else null
    }

    fun <T : DaoModel<T>> getMany(model: Class<T>, filter: Bson? = null): List<T> {
        return if (filter == null) {
            getCollection(model).find().toList()
        } else {
            getCollection(model).find(filter).toList()
        }
    }


    fun <T : DaoModel<T>> remove(id: Id<T>, model: Class<T>): Boolean {
        return getCollection(model).deleteOne(DaoModel<T>::id.eq(id)).deletedCount > 0
    }

    private fun <T : DaoModel<T>> getCollection(model: Class<T>): MongoCollection<T> {
        return mongoProvider.database.getCollection(KMongoUtil.defaultCollectionName(model.kotlin), model)
    }
}