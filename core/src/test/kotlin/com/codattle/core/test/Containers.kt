package com.codattle.core.test

object Containers {

    val mongoContainer = MongoContainer()
    val rabbitMQContainer = RabbitMQContainer()

    init {
        mongoContainer.start()
        rabbitMQContainer.start()
    }
}