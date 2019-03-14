package com.codattle.mainservice.service

import com.codattle.mainservice.dao.Dao
import com.codattle.mainservice.model.Simulation
import com.rabbitmq.client.Channel
import com.rabbitmq.client.Connection
import com.rabbitmq.client.ConnectionFactory
import javax.annotation.PreDestroy
import javax.inject.Singleton

@Singleton
class SimulationService(
        private val dao: Dao
) {

    companion object {
        const val QUEUE_NAME = "simulate"
    }

    private val connection: Connection
    private val channel: Channel;

    init {
        val factory = ConnectionFactory()
        factory.host = "localhost"
        connection = factory.newConnection()
        channel = connection.createChannel()
        channel.queueDeclare(QUEUE_NAME, false, false, false, null)
    }

    @PreDestroy
    private fun destroy() {
        connection.close()
    }

    fun executeSimulation(simulationName: String) {
        channel.basicPublish("", QUEUE_NAME, null, simulationName.toByteArray())

        val simulation = Simulation()
        simulation.name = simulationName
        dao.save(simulation)
    }
}