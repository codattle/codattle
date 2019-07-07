package com.codattle.core.dao

import com.fasterxml.jackson.databind.ObjectMapper
import io.micronaut.context.annotation.Factory
import javax.inject.Singleton

@Factory
class ObjectMapperFactory {

    @Singleton
    private fun objectMapper(): ObjectMapper {
        return ObjectMapper()
    }
}