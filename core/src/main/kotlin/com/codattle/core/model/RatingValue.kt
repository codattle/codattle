package com.codattle.core.model

data class RatingValue(val value: Int) {

    companion object {
        const val MIN_VALUE = 1
        const val MAX_VALUE = 5
    }

    init {
        require(value in MIN_VALUE..MAX_VALUE) { "Rating value must be between $MIN_VALUE nad $MAX_VALUE" }
    }
}