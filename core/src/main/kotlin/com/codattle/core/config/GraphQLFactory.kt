package com.codattle.core.config

import com.codattle.core.resolver.Mutation
import com.codattle.core.resolver.Query
import com.codattle.core.scalar.ObjectIdScalar
import com.coxautodev.graphql.tools.SchemaParser
import graphql.GraphQL
import io.micronaut.context.annotation.Bean
import io.micronaut.context.annotation.Factory
import io.micronaut.core.io.ResourceResolver
import javax.inject.Singleton

@Factory
class GraphQLFactory {

    @Bean
    @Singleton
    fun graphQL(query: Query, mutation: Mutation): GraphQL {

        val graphQLSchema = SchemaParser.newParser()
                .file("schema.graphqls")
                .resolvers(query, mutation)
                .scalars(ObjectIdScalar())
                .build()
                .makeExecutableSchema()

        return GraphQL.newGraphQL(graphQLSchema).build()
    }
}
