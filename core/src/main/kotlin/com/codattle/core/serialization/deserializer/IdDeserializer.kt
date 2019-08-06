package com.codattle.core.serialization.deserializer

import com.codattle.core.dao.common.Id
import com.fasterxml.jackson.core.JsonParser
import com.fasterxml.jackson.databind.DeserializationContext
import com.fasterxml.jackson.databind.deser.std.StdDeserializer

class IdDeserializer(clazz: Class<*>? = null) : StdDeserializer<Id<*>>(clazz) {

    override fun deserialize(parser: JsonParser, context: DeserializationContext): Id<*> {
        return Id.unchecked(parser.valueAsString)
    }
}