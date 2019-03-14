package com.codattle.mainservice.dto

import javax.validation.constraints.NotBlank
import javax.validation.constraints.NotNull

class SimulationDTO {

    @NotBlank
    @NotNull
    var name: String? = null


}