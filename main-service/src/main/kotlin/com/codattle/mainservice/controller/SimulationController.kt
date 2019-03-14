package com.codattle.mainservice.controller

import com.codattle.mainservice.dto.SimulationDTO
import com.codattle.mainservice.service.SimulationService
import io.micronaut.http.annotation.Body
import io.micronaut.http.annotation.Controller
import io.micronaut.http.annotation.Post
import io.micronaut.validation.Validated
import javax.validation.Valid

@Validated
@Controller("/simulation")
class SimulationController(
        private val simulationService: SimulationService
) {

    @Post
    fun executeSimulation(@Valid @Body simulation: SimulationDTO) {
        simulationService.executeSimulation(simulation.name!!)
    }
}