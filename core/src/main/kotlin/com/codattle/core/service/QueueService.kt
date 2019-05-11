package com.codattle.core.service

import com.rabbitmq.client.Channel
import com.rabbitmq.client.Connection
import com.rabbitmq.client.ConnectionFactory
import io.micronaut.context.annotation.Value
import javax.annotation.PreDestroy

class QueueService(@Value("\${codattle.queue.host:localhost}") private val queueHost: String,
                   @Value("\${codattle.queue.port:5672}") private val queuePort: Int) {

    private val connection: Connection
    private val channel: Channel

    init {
        val factory = ConnectionFactory()
        factory.host = queueHost
        factory.port = queuePort
        connection = factory.newConnection()
        channel = connection.createChannel()

    }

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