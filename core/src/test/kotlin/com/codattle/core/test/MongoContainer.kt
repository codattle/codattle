package com.codattle.core.test

import org.testcontainers.containers.GenericContainer

class MongoContainer : GenericContainer<MongoContainer>("mongo:4.0.6") {

    companion object {
        const val username = "mongo"
        const val password = "mongo"
        const val authorizationDatabase = "admin"
    }

    init {
        withExposedPorts(27017)
        withEnv("MONGO_INITDB_ROOT_USERNAME", username)
        withEnv("MONGO_INITDB_ROOT_PASSWORD", password)
    }
}