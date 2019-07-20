package com.codattle.core.dao

interface DaoModel<T : DaoModel<T>> {

    var id: Id<T>?
}