package com.codattle.core.scalar

import com.codattle.core.dao.Id
import graphql.language.IntValue
import graphql.language.StringValue
import graphql.schema.*

class IdScalar : GraphQLScalarType("ID", "Id", object : Coercing<Id<*>, String> {

    override fun parseValue(input: Any?): Id<*> {
        return when (input) {
            is Id<*> -> input
            is Int -> Id.unchecked(input.toString())
            is String -> parseId(input, ::CoercingParseValueException)
            else -> throw CoercingParseValueException("Expected 'Int' or 'com.codattle.core.dao.Id' but was '${input!!.javaClass.name}'.")
        }
    }

    override fun parseLiteral(input: Any?): Id<*> {
        return when (input) {
            is IntValue -> Id.unchecked(input.value.toString())
            is StringValue -> parseId(input.value, ::CoercingParseLiteralException)
            else -> throw CoercingParseLiteralException("Expected AST type 'IntValue' or 'StringValue' but was '${input!!.javaClass.name}'.")
        }
    }

    override fun serialize(dataFetcherResult: Any?): String {
        if (dataFetcherResult is Id<*>) {
            return dataFetcherResult.id.toString()
        } else {
            throw CoercingSerializeException("Expected 'com.codattle.core.dao.Id' but was '${dataFetcherResult!!.javaClass.name}'.")
        }
    }

    private fun parseId(value: String, exceptionMaker: (value: String) -> RuntimeException): Id<*> {
        try {
            return Id.unchecked(value)
        } catch (ex: RuntimeException) {
            throw exceptionMaker("Unable to parse '$value' as ID.")
        }
    }
})