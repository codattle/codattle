package com.codattle.core.test

import org.testcontainers.containers.GenericContainer

class RabbitMQContainer : GenericContainer<RabbitMQContainer>("rabbitmq:3.7.13-alpine") {

    init {
        withExposedPorts(5672)
    }
}