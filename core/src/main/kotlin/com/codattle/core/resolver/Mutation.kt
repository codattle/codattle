package com.codattle.core.resolver

import com.codattle.core.dao.Dao
import com.codattle.core.model.Simulation
import com.codattle.core.service.SimulationService
import com.coxautodev.graphql.tools.GraphQLMutationResolver
import org.bson.types.ObjectId
import javax.inject.Singleton

@Singleton
class Mutation(private val dao: Dao, private val simulationService: SimulationService) : GraphQLMutationResolver {

    fun executeSimulation(name: String): Simulation {
        val id = simulationService.executeSimulation(name)
        return dao.get(id, Simulation::class.java)
    }

    fun removeSimulation(id: ObjectId): Boolean {
        return dao.remove(id, Simulation::class.java)
    }
}