package com.codattle.core.serialization.deserializer

import com.fasterxml.jackson.core.JsonParser
import com.fasterxml.jackson.databind.DeserializationContext
import com.fasterxml.jackson.databind.deser.std.StdDeserializer
import io.micronaut.http.MediaType

class MediaTypeDeserializer(clazz: Class<*>? = null) : StdDeserializer<MediaType>(clazz) {

    override fun deserialize(parser: JsonParser, context: DeserializationContext): MediaType {
        return MediaType(parser.valueAsString)
    }
}