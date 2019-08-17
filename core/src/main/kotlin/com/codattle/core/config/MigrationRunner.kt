package com.codattle.core.config

import com.codattle.core.dao.common.MongoProvider
import com.github.cloudyrock.mongock.MongockBuilder
import io.micronaut.context.event.BeanCreatedEvent
import io.micronaut.context.event.BeanCreatedEventListener
import javax.inject.Singleton


@Singleton
class MigrationRunner : BeanCreatedEventListener<MongoProvider> {

    override fun onCreated(event: BeanCreatedEvent<MongoProvider>): MongoProvider {
        MongockBuilder(event.bean.client, event.bean.database.name, "com.codattle.core.migration")
                .setLockQuickConfig()
                .build()
                .execute()
        return event.bean
    }
}