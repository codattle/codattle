package com.codattle.core.dao

import com.codattle.core.serialization.deserializer.IdDeserializer
import com.codattle.core.serialization.serializer.IdSerializer
import com.fasterxml.jackson.databind.module.SimpleModule
import com.mongodb.*
import com.mongodb.client.MongoCollection
import com.mongodb.client.MongoDatabase
import io.micronaut.context.annotation.Value
import org.bson.codecs.Codec
import org.bson.codecs.configuration.CodecRegistries
import org.litote.kmongo.KMongo
import org.litote.kmongo.util.KMongoCodecProvider
import org.litote.kmongo.util.KMongoConfiguration
import org.litote.kmongo.util.KMongoUtil
import org.litote.kmongo.util.ObjectMappingConfiguration
import java.util.stream.Stream
import javax.annotation.PreDestroy
import javax.inject.Singleton
import kotlin.streams.toList

@Singleton
class MongoProvider(
        @Value("\${codattle.mongodb.host:localhost}") private val host: String,
        @Value("\${codattle.mongodb.port:27017}") private val port: Int,
        @Value("\${codattle.mongodb.authentication.username:mongo}") private val authenticationUsername: String,
        @Value("\${codattle.mongodb.authentication.password:mongo}") private val authenticationPassword: String,
        @Value("\${codattle.mongodb.authentication.database:admin}") private val authenticationDatabase: String
) {

    companion object {
        private const val DATABASE = "default"
    }

    val database: MongoDatabase

    private val client: MongoClient

    init {
        val address = ServerAddress(host, port)
        val credentials = MongoCredential.createCredential(authenticationUsername, authenticationDatabase, authenticationPassword.toCharArray())

        val module = SimpleModule()
                .addSerializer(Id::class.java, IdSerializer())
                .addDeserializer(Id::class.java, IdDeserializer())
        KMongoConfiguration.registerBsonModule(module)

        client = KMongo.createClient(address, listOf(credentials))
        database = client.getDatabase(DATABASE)
    }

    @PreDestroy
    private fun destroy() {
        client.close()
    }
}