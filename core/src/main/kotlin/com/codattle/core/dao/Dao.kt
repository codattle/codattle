package com.codattle.core.dao

import com.mongodb.MongoClient
import com.mongodb.MongoClientOptions
import com.mongodb.MongoCredential
import com.mongodb.ServerAddress
import dev.morphia.Datastore
import dev.morphia.Morphia
import dev.morphia.mapping.Mapper
import io.micronaut.context.annotation.Value
import org.bson.types.ObjectId
import javax.annotation.PreDestroy
import javax.inject.Singleton

@Singleton
class Dao(
        @Value("\${codattle.mongodb.host:localhost}") private val host: String,
        @Value("\${codattle.mongodb.port:27017}") private val port: Int,
        @Value("\${codattle.mongodb.authentication.username:mongo}") private val authenticationUsername: String,
        @Value("\${codattle.mongodb.authentication.password:mongo}") private val authenticationPassword: String,
        @Value("\${codattle.mongodb.authentication.database:admin}") private val authenticationDatabase: String
) {

    companion object {
        private const val DATABASE = "default"
    }

    private val mongoClient: MongoClient
    private val morphia = Morphia()
    private val datastore: Datastore

    init {
        val credential = MongoCredential.createCredential(authenticationUsername, authenticationDatabase, authenticationPassword.toCharArray())
        mongoClient = MongoClient(ServerAddress(host, port), credential, MongoClientOptions.builder().build())

        morphia.mapPackage("com.codattle.core.dto")

        datastore = morphia.createDatastore(mongoClient, DATABASE)
        datastore.ensureIndexes()
    }

    @PreDestroy
    private fun destroy() {
        mongoClient.close()
    }

    fun <T : DaoModel> save(entity: T): ObjectId {
        return datastore.save(entity).id as ObjectId
    }

    fun <T : DaoModel> saveAndGet(entity: T): T {
        val id = save(entity)
        return get(id, entity.javaClass)
    }

    fun <T> get(id: ObjectId, clazz: Class<T>): T {
        return datastore.get(clazz, id)
    }

    fun <T> getWithFields(id: ObjectId, clazz: Class<T>, includedFields: List<String>): T {
        var query = datastore.find(clazz).field(Mapper.ID_KEY).equal(id)
        for (includedField in includedFields) {
            query = query.project(includedField, true)
        }
        return query.get()
    }

    fun <T> getAll(clazz: Class<T>): List<T> {
        return datastore.find(clazz).asList()
    }

    fun <T> getAllWithFieldEqual(clazz: Class<T>, field: String, value: Any): List<T> {
        return datastore.find(clazz).field(field).equal(value).asList()
    }

    fun <T> remove(id: ObjectId, clazz: Class<T>): Boolean {
        return datastore.delete(clazz, id).n > 0
    }

}