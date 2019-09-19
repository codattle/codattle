package com.codattle.core.dao.common

import com.codattle.core.dao.Sequence
import com.mongodb.client.MongoCollection
import com.mongodb.client.model.FindOneAndUpdateOptions
import com.mongodb.client.model.ReplaceOptions
import com.mongodb.client.model.ReturnDocument
import org.bson.conversions.Bson
import org.litote.kmongo.and
import org.litote.kmongo.eq
import org.litote.kmongo.findOne
import org.litote.kmongo.projection
import org.litote.kmongo.util.KMongoUtil
import java.lang.IllegalArgumentException
import java.time.Instant
import javax.inject.Singleton
import kotlin.reflect.KProperty

@Singleton
class DaoUtils(
        private val mongoProvider: MongoProvider,
        private val sequence: Sequence
) {

    fun <T : DaoModel<T>> save(entity: T): T {
        return save(entity.toBuilder(), entity.javaClass)
    }

    inline fun <reified T : DaoModel<T>> save(entity: DaoModelBuilder<T>): T {
        return save(entity, T::class.java)
    }

    fun <T : DaoModel<T>> save(entity: DaoModelBuilder<T>, model: Class<T>): T {
        fillAuditFields(entity)

        return if (entity.id == null) {
            entity.id = Id(sequence.getNext(getCollectionName(model)))
            entity.build().apply { getCollection(model).insertOne(this) }
        } else {
            entity.build().apply { getCollection(model).replaceOne(DaoModel<T>::id eq entity.id, this) }
        }
    }

    inline fun <reified T : DaoModel<T>> insertOrReplace(entity: DaoModelBuilder<T>, filter: Bson): T {
        return insertOrReplace(entity, T::class.java, filter)
    }

    fun <T : DaoModel<T>> insertOrReplace(entity: DaoModelBuilder<T>, model: Class<T>, filter: Bson): T {
        if (entity.id != null) {
            throw IllegalArgumentException("Only new entity can be inserted")
        }

        entity.id = Id(sequence.getNext(getCollectionName(model)))
        fillAuditFields(entity)

        return entity.build().apply { getCollection(model).replaceOne(filter, this, ReplaceOptions().upsert(true)) }
    }

    inline fun <reified T : DaoModel<T>> get(id: Id<T>): T? {
        return get(id, T::class.java)
    }

    fun <T : DaoModel<T>> get(id: Id<T>, model: Class<T>): T? {
        return getCollection(model).findOne(DaoModel<T>::id.eq(id))
    }

    inline fun <T : DaoModel<T>, reified F> getWithFields(id: Id<T>, model: Class<T>, projection: KProperty<F>): F? {
        return getCollection(model).projection(projection, DaoModel<T>::id eq id).first()
    }

    inline fun <reified T : DaoModel<T>> exists(id: Id<T>): T? {
        return getCollection(T::class.java).
    }

    inline fun <reified T : DaoModel<T>> getMany(filter: Bson? = null): List<T> {
        return if (filter == null) {
            getCollection(T::class.java).find().toList()
        } else {
            getCollection(T::class.java).find(filter).toList()
        }
    }

    fun <T : DaoModel<T>> getMany(model: Class<T>, filter: Bson? = null): List<T> {
        return if (filter == null) {
            getCollection(model).find().toList()
        } else {
            getCollection(model).find(filter).toList()
        }
    }

    inline fun <reified T : DaoModel<T>> findAndModify(id: Id<T>, update: Bson): T? {
        return findAndModify(T::class.java, id, update)
    }

    fun <T : DaoModel<T>> findAndModify(model: Class<T>, id: Id<T>, update: Bson): T? {
        return findAndModify(model, DaoModel<T>::id eq id, update)
    }

    fun <T : DaoModel<T>> findAndModify(model: Class<T>, filter: Bson, update: Bson): T? {
        val options = FindOneAndUpdateOptions().returnDocument(ReturnDocument.AFTER)
        return getCollection(model).findOneAndUpdate(filter, update, options)
    }

    fun <T : DaoModel<T>> remove(id: Id<T>, model: Class<T>): Boolean {
        return getCollection(model).deleteOne(DaoModel<T>::id eq id).deletedCount > 0
    }

    fun <T : DaoModel<T>> getCollection(model: Class<T>): MongoCollection<T> {
        return mongoProvider.database.getCollection(getCollectionName(model), model)
    }

    private fun <T : DaoModel<T>> getCollectionName(model: Class<T>): String {
        return KMongoUtil.defaultCollectionName(model.kotlin)
    }

    private fun fillAuditFields(entity: DaoModelBuilder<*>) {
        if (entity.creationDate == null) {
            entity.creationDate = Instant.now()
        }
    }
}