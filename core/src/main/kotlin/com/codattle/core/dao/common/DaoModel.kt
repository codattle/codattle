package com.codattle.core.dao.common

import java.time.Instant

interface DaoModel<T : DaoModel<T>> {

    val id: Id<T>
    val creationDate: Instant

    fun toBuilder(): DaoModelBuilder<T>
}