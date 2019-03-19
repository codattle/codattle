package com.codattle.core.model

import com.codattle.core.dao.DaoModel
import dev.morphia.annotations.Entity

@Entity
class Simulation : DaoModel() {

    var name: String? = null

}