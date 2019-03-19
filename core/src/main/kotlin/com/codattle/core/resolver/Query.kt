package com.codattle.core.resolver

import com.codattle.core.dao.Dao
import com.codattle.core.model.Simulation
import com.coxautodev.graphql.tools.GraphQLQueryResolver
import javax.inject.Singleton

@Singleton
class Query(private val dao: Dao) : GraphQLQueryResolver {

    fun simulations(): List<Simulation> {
        return dao.getAll(Simulation::class.java)
    }

}