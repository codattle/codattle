package com.codattle.core.model

import com.codattle.annotation.processing.annotation.GenerateDaoModelBuilder
import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils
import com.codattle.core.model.utils.ModelUtils.nonNull
import java.time.Instant
import java.time.ZonedDateTime

@GenerateDaoModelBuilder
data class User(
        override val id: Id<User>,
        override val creationDate: Instant,
        val username: String
) : DaoModel<User> {

    companion object {
        // TODO: delete after implementing authentication be Keycloak
        val DEFAULT_USER = Id<User>("DEFAULT_USER")

        fun builder(): UserBuilder {
            return UserBuilder.withoutDefault()
        }
    }

    override fun toBuilder(): UserBuilder {
        return UserBuilder.fromModel(this)
    }
}