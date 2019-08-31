package com.codattle.core.scalar

import com.codattle.core.dao.common.Id
import com.codattle.core.model.RatingValue
import graphql.language.IntValue
import graphql.schema.*
import java.math.BigInteger

class RatingValueScalar : GraphQLScalarType("RatingValue", "Number between ${RatingValue.MIN_VALUE} and ${RatingValue.MAX_VALUE}", object : Coercing<RatingValue, Int> {

    override fun parseValue(input: Any): RatingValue {
        return when (input) {
            is RatingValue -> input
            is Int -> parseRatingValue(input.toBigInteger(), ::CoercingParseValueException)
            else -> throw CoercingParseValueException("Expected 'Int' or 'com.codattle.core.model.RatingValue' but was '${input.javaClass.name}'.")
        }
    }

    override fun parseLiteral(input: Any): RatingValue {
        return when (input) {
            is IntValue -> parseRatingValue(input.value, ::CoercingParseLiteralException)
            else -> throw CoercingParseLiteralException("Expected AST type 'IntValue' or 'StringValue' but was '${input.javaClass.name}'.")
        }
    }

    override fun serialize(dataFetcherResult: Any): Int {
        if (dataFetcherResult is RatingValue) {
            return dataFetcherResult.value
        } else {
            throw CoercingSerializeException("Expected 'com.codattle.core.model.RatingValue' but was '${dataFetcherResult.javaClass.name}'.")
        }
    }

    private fun parseRatingValue(value: BigInteger, exceptionMaker: (value: String) -> RuntimeException): RatingValue {
        try {
            return RatingValue(value.toInt())
        } catch (ex: RuntimeException) {
            throw exceptionMaker("Unable to parse '$value' as RatingValue.")
        }
    }
})