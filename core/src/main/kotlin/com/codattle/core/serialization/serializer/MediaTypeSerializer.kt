package com.codattle.core.serialization.serializer

import com.fasterxml.jackson.core.JsonGenerator
import com.fasterxml.jackson.databind.SerializerProvider
import com.fasterxml.jackson.databind.ser.std.StdSerializer
import io.micronaut.http.MediaType

class MediaTypeSerializer(clazz: Class<MediaType>? = null) : StdSerializer<MediaType>(clazz) {

    override fun serialize(value: MediaType, generator: JsonGenerator, provider: SerializerProvider) {
        generator.writeString(value.toString())
    }
}