package com.codattle.core.dao

import com.mongodb.MongoClient
import com.mongodb.MongoCredential
import com.mongodb.ServerAddress
import com.mongodb.client.MongoDatabase
import io.micronaut.context.annotation.Value
import org.litote.kmongo.KMongo
import javax.annotation.PreDestroy
import javax.inject.Singleton

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
        val address = ServerAddress(host, port);
        val credentials = MongoCredential.createCredential(authenticationUsername, authenticationDatabase, authenticationPassword.toCharArray())
        client = KMongo.createClient(address, listOf(credentials))
        database = client.getDatabase(DATABASE)
    }

    @PreDestroy
    private fun destroy() {
        client.close()
    }
}