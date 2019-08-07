package com.codattle.core.serialization.serializer

import com.codattle.core.dao.common.Id
import com.fasterxml.jackson.core.JsonGenerator
import com.fasterxml.jackson.databind.JavaType
import com.fasterxml.jackson.databind.SerializerProvider
import com.fasterxml.jackson.databind.jsonFormatVisitors.JsonFormatVisitorWrapper
import com.fasterxml.jackson.databind.ser.std.StdSerializer

class IdSerializer(clazz: Class<Id<*>>? = null) : StdSerializer<Id<*>>(clazz) {

    override fun serialize(value: Id<*>, generator: JsonGenerator, provider: SerializerProvider) {
        generator.writeString(value.id)
    }

    override fun acceptJsonFormatVisitor(visitor: JsonFormatVisitorWrapper, typeHint: JavaType?) {
        visitor.expectStringFormat(typeHint)
    }
}