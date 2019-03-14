package com.codattle.mainservice.model

import com.codattle.mainservice.dao.Collection
import com.codattle.mainservice.dao.DaoModel

class Simulation : DaoModel() {

    var name: String? = null

    override fun getDefaultCollection(): Collection? = Collection.SIMULATION
}