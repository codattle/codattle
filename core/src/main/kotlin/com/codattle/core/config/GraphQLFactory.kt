package com.codattle.core.config

import com.codattle.core.scalar.IdScalar
import com.codattle.core.scalar.RatingValueScalar
import com.coxautodev.graphql.tools.GraphQLResolver
import com.coxautodev.graphql.tools.SchemaParser
import graphql.GraphQL
import io.micronaut.context.annotation.Bean
import io.micronaut.context.annotation.Factory
import javax.inject.Singleton

@Factory
class GraphQLFactory {

    @Bean
    @Singleton
    fun graphQL(resolvers: List<GraphQLResolver<*>>): GraphQL {

        val graphQLSchema = SchemaParser.newParser()
                .file("schema.graphqls")
                .resolvers(resolvers)
                .scalars(IdScalar(), RatingValueScalar())
                .dictionary()
                .build()
                .makeExecutableSchema()

        return GraphQL.newGraphQL(graphQLSchema).build()
    }
}
