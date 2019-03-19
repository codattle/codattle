package com.codattle.core.service

import com.codattle.core.dao.Dao
import com.codattle.core.model.Simulation
import com.rabbitmq.client.Channel
import com.rabbitmq.client.Connection
import com.rabbitmq.client.ConnectionFactory
import io.micronaut.context.annotation.Value
import org.bson.types.ObjectId
import javax.annotation.PreDestroy
import javax.inject.Singleton

@Singleton
class SimulationService(
        private val dao: Dao,
        @Value("\${codattle.queue.host:localhost}") private val queueHost: String,
        @Value("\${codattle.queue.port:5672}") private val queuePort: Int
) {

    companion object {
        const val QUEUE_NAME = "simulate"
    }

    private val connection: Connection
    private val channel: Channel

    init {
        val factory = ConnectionFactory()
        factory.host = queueHost
        factory.port = queuePort
        connection = factory.newConnection()
        channel = connection.createChannel()
        channel.queueDeclare(QUEUE_NAME, false, false, false, null)
    }

    @PreDestroy
    private fun destroy() {
        connection.close()
    }

    fun executeSimulation(simulationName: String): ObjectId {
        channel.basicPublish("", QUEUE_NAME, null, simulationName.toByteArray())

        val simulation = Simulation()
        simulation.name = simulationName
        return dao.save(simulation)
    }
}