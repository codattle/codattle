package com.codattle.core.dao.common

data class Id<T>(val value: String) {

    companion object {
        fun unchecked(value: String): Id<*> {
            return Id<Any>(value)
        }
    }

    override fun toString(): String {
        return value
    }
}