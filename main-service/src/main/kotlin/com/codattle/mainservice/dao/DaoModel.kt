package com.codattle.mainservice.dao

import com.fasterxml.jackson.annotation.JsonIgnore

open class DaoModel {

    @JsonIgnore
    open fun getDefaultCollection(): Collection? = null
}