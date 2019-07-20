package com.codattle.core.dao

import com.fasterxml.jackson.databind.ObjectMapper
import com.mongodb.client.MongoCollection
import com.mongodb.client.model.Filters
import com.mongodb.client.model.FindOneAndUpdateOptions
import com.mongodb.client.model.ReturnDocument
import org.bson.Document
import org.bson.conversions.Bson
import org.litote.kmongo.eq
import org.litote.kmongo.findOne
import org.litote.kmongo.projection
import org.litote.kmongo.util.KMongoUtil
import javax.inject.Singleton
import kotlin.reflect.KProperty
import kotlin.reflect.full.memberProperties

@Singleton
class Dao(
        private val mongoProvider: MongoProvider,
        private val sequence: Sequence
) {

    fun <T : DaoModel<T>> save(entity: T): T {
        if (entity.id == null) {
            entity.id = Id(sequence.getNext(KMongoUtil.defaultCollectionName(entity.javaClass.kotlin)))
            getCollection(entity.javaClass).insertOne(entity)
        } else {
            getCollection(entity.javaClass).replaceOne(DaoModel<T>::id eq entity.id, entity)
        }
        return entity
    }

    fun <T : DaoModel<T>> get(id: Id<T>, model: Class<T>): T? {
        return getCollection(model).findOne(DaoModel<T>::id.eq(id))
    }

    inline fun <T : DaoModel<T>, reified F> getWithFields(id: Id<T>, model: Class<T>, projection: KProperty<F>): F? {
        return getCollection(model).projection(projection, DaoModel<T>::id eq id).first()
    }

    fun <T : DaoModel<T>> getMany(model: Class<T>, filter: Bson? = null): List<T> {
        return if (filter == null) {
            getCollection(model).find().toList()
        } else {
            getCollection(model).find(filter).toList()
        }
    }

    fun <T : DaoModel<T>> findAndModify(model: Class<T>, filter: Bson, update: Bson): T? {
        val options = FindOneAndUpdateOptions().returnDocument(ReturnDocument.AFTER)
        return getCollection(model).findOneAndUpdate(filter, update, options)
    }

    fun <T : DaoModel<T>> remove(id: Id<T>, model: Class<T>): Boolean {
        return getCollection(model).deleteOne(DaoModel<T>::id eq id).deletedCount > 0
    }

    fun <T : DaoModel<T>> getCollection(model: Class<T>): MongoCollection<T> {
        return mongoProvider.database.getCollection(KMongoUtil.defaultCollectionName(model.kotlin), model)
    }
}