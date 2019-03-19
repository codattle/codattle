package com.codattle.core.scalar

import graphql.language.StringValue
import graphql.schema.*
import org.bson.types.ObjectId

class ObjectIdScalar : GraphQLScalarType("ID", "Mongo Object Id", object : Coercing<ObjectId, String> {

    override fun parseValue(input: Any?): ObjectId {
        return when (input) {
            is ObjectId -> input
            is String -> parseObjectId(input, ::CoercingParseValueException)
            else -> throw CoercingParseValueException("Expected 'String' or 'org.bson.types.ObjectId' but was '${input!!.javaClass.name}'.")
        }
    }

    override fun parseLiteral(input: Any?): ObjectId {
        if (input is StringValue) {
            return parseObjectId(input.value, ::CoercingParseLiteralException)
        } else {
            throw CoercingParseLiteralException("Expected AST type 'StringValue' but was '${input!!.javaClass.name}'.")
        }
    }

    override fun serialize(dataFetcherResult: Any?): String {
        return when (dataFetcherResult) {
            is ObjectId -> dataFetcherResult.toHexString()
            is String -> parseObjectId(dataFetcherResult, ::CoercingSerializeException).toHexString()
            else -> throw CoercingSerializeException("Expected 'String' or 'org.bson.types.ObjectId' but was '${dataFetcherResult!!.javaClass.name}'.")
        }
    }

    private fun parseObjectId(value: String, exceptionMaker: (value: String) -> RuntimeException): ObjectId {
        try {
            return ObjectId(value)
        } catch (ex: RuntimeException) {
            throw exceptionMaker("Unable to parse '$value' as ID.")
        }
    }

})