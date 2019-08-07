package com.codattle.core.dao.common

data class Id<T>(val id: String) {

    companion object {
        fun unchecked(id: String): Id<*> {
            return Id<Any>(id)
        }
    }

    override fun toString(): String {
        return id
    }
}