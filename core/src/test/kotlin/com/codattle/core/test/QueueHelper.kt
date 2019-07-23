package com.codattle.core.test

import com.rabbitmq.client.*
import io.micronaut.context.annotation.Value
import io.netty.util.concurrent.Promise
import java.io.IOException
import java.util.concurrent.CountDownLatch
import java.util.concurrent.TimeUnit
import java.util.concurrent.locks.Condition
import java.util.concurrent.locks.ReentrantLock
import javax.annotation.PreDestroy
import javax.inject.Singleton

@Singleton
class QueueHelper(@Value("\${codattle.queue.host}") private val queueHost: String,
                  @Value("\${codattle.queue.port}") private val queuePort: Int) {

    private val connection: Connection

    companion object {
        const val SIMULATION = "simulation"
    }

    init {
        val factory = ConnectionFactory()
        factory.host = queueHost
        factory.port = queuePort
        connection = factory.newConnection()
    }

    @PreDestroy
    private fun destroy() {
        connection.close()
    }

    fun clean() {
        removeAllMessages(SIMULATION)
    }

    fun readMessage(queueName: String, timeout: Long = 5): ByteArray? {
        var response: ByteArray? = null
        val latch = CountDownLatch(1)

        val channel = connection.createChannel()
        channel.basicConsume(queueName, true, object : DefaultConsumer(channel) {
            override fun handleDelivery(consumerTag: String?, envelope: Envelope?, properties: AMQP.BasicProperties?, body: ByteArray?) {
                response = body
                latch.countDown()
            }
        })

        latch.await(timeout, TimeUnit.SECONDS)

        return response
    }

    fun countMessages(queueName: String): Long {
        return connection.createChannel().messageCount(queueName)
    }

    fun doesQueueExist(queueName: String): Boolean {
        return try {
            connection.createChannel().queueDeclarePassive(queueName)
            true
        } catch (ex: IOException) {
            false
        }
    }

    private fun removeAllMessages(queueName: String) {
        try {
            connection.createChannel().queuePurge(queueName)
        } catch (ex: IOException) {
            // queue doesn't exist
        }
    }
}
