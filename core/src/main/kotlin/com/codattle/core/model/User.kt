package com.codattle.core.model

import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils
import com.codattle.core.model.utils.ModelUtils.nonNull
import java.time.Instant
import java.time.ZonedDateTime

data class User(
        override val id: Id<User>,
        override val creationDate: Instant,
        val username: String
) : DaoModel<User> {

    companion object {
        // TODO: delete after implementing authentication be Keycloak
        val DEFAULT_USER = Id<User>("DEFAULT_USER")
    }

    data class Builder(
            override var id: Id<User>? = null,
            override var creationDate: Instant? = null,
            var username: String? = null
    ) : DaoModelBuilder<User> {

        override fun build(): User {
            return User(
                    id = nonNull(id, "id"),
                    creationDate = nonNull(creationDate, "creationDate"),
                    username = nonNull(username, "username")
            )
        }
    }

    override fun toBuilder(): Builder {
        return Builder(
                id = id,
                creationDate = creationDate,
                username = username
        )
    }
}