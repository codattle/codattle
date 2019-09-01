package com.codattle.core.model

data class RatingValue(val value: Int) {

    companion object {
        const val MIN_VALUE = 1
        const val MAX_VALUE = 5
    }

    init {
        if (value < MIN_VALUE || value > MAX_VALUE) {
            throw IllegalArgumentException("Rating value must be between $MIN_VALUE nad $MAX_VALUE")
        }
    }
}