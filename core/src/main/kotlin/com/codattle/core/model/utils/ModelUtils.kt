package com.codattle.core.model.utils

object ModelUtils {

    fun <T> nonNull(value: T?, fieldName: String): T {
        return value ?: throw IllegalStateException("Field $fieldName cannot be null while building")
    }
}