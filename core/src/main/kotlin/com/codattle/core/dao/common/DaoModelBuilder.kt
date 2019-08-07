package com.codattle.core.dao.common

import java.time.Instant

interface DaoModelBuilder<T : DaoModel<T>> {

    var id: Id<T>?
    var creationDate: Instant?
    var modificationDate: Instant?

    fun build(): T

    fun getModelClass(): Class<T>
}