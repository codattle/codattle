package com.codattle.core.service

import com.rabbitmq.client.ConnectionFactory
import io.micronaut.context.annotation.Value
import javax.annotation.PreDestroy
import javax.inject.Singleton

@Singleton
class QueueService(@Value("\${codattle.queue.host:localhost}") private val queueHost: String,
                   @Value("\${codattle.queue.port:5672}") private val queuePort: Int) {

    private val connection = ConnectionFactory().apply { host = queueHost; port = queuePort }.newConnection()
    private val channel = connection.createChannel()

    @PreDestroy
    private fun destroy() {
        connection.close()
    }

    fun createQueue(queueName: String) {
        channel.queueDeclare(queueName, false, false, false, null)
    }

    fun sendMessage(queueName: String, message: ByteArray) {
        channel.basicPublish("", queueName, null, message)
    }
}