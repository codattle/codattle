package com.codattle.mainservice.dao

import com.fasterxml.jackson.databind.ObjectMapper
import com.mongodb.MongoClient
import com.mongodb.MongoClientOptions
import com.mongodb.MongoCredential
import com.mongodb.ServerAddress
import com.mongodb.client.MongoDatabase
import org.bson.Document
import javax.annotation.PreDestroy
import javax.inject.Singleton

@Singleton
class Dao {

    companion object {
        private const val DATABASE = "default"
    }

    private val mongoClient: MongoClient
    private val database: MongoDatabase
    private val objectMapper = ObjectMapper()

    init {
        val credential = MongoCredential.createCredential("mongo", "admin", "mongo".toCharArray())
        mongoClient = MongoClient(ServerAddress("localhost", 27017), credential, MongoClientOptions.builder().build())
        database = mongoClient.getDatabase(DATABASE)
    }

    @PreDestroy
    private fun destroy() {
        mongoClient.close()
    }

    fun save(model: DaoModel, collectionName: String? = null) {
        val json = objectMapper.writeValueAsString(model)
        val document = Document.parse(json)!!
        val collection = collectionName ?: model.getDefaultCollection()?.collectionName
        if (collection != null) {
            database.getCollection(collection).insertOne(document)
        } else {
            throw IllegalArgumentException("Class '${model.javaClass.name}' doesn't have associated collection and collection name wasn't provider")
        }
    }

}