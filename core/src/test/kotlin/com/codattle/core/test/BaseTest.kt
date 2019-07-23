package com.codattle.core.test

import io.micronaut.test.annotation.MicronautTest
import io.micronaut.test.support.TestPropertyProvider
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.TestInstance
import org.testcontainers.junit.jupiter.Testcontainers
import javax.inject.Inject

@MicronautTest
@TestInstance(TestInstance.Lifecycle.PER_CLASS)
@Testcontainers
open class BaseTest : TestPropertyProvider {

    @Inject
    protected lateinit var databasePopulator: DatabasePopulator

    @Inject
    protected lateinit var queueHelper: QueueHelper

    override fun getProperties(): MutableMap<String, String> = mutableMapOf(
            "codattle.queue.host" to Containers.rabbitMQContainer.containerIpAddress,
            "codattle.queue.port" to Containers.rabbitMQContainer.firstMappedPort.toString(),
            "codattle.mongodb.host" to Containers.mongoContainer.containerIpAddress,
            "codattle.mongodb.port" to Containers.mongoContainer.firstMappedPort.toString(),
            "codattle.mongodb.authentication.username" to MongoContainer.username,
            "codattle.mongodb.authentication.password" to MongoContainer.password,
            "codattle.mongodb.authentication.database" to MongoContainer.authorizationDatabase
    )

    @BeforeEach
    fun clean() {
        databasePopulator.clean()
        queueHelper.clean()
    }
}