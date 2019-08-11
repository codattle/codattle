package com.codattle.core.model

import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils.nonNull
import io.micronaut.http.MediaType
import java.time.Instant

data class File(
        override val id: Id<File>,
        override val creationDate: Instant,
        override val modificationDate: Instant,
        val name: String,
        val owner: Id<User>,
        val contentType: MediaType
) : DaoModel<File> {

    data class Builder(
            override var id: Id<File>? = null,
            override var creationDate: Instant? = null,
            override var modificationDate: Instant? = null,
            var name: String? = null,
            var owner: Id<User>? = null,
            val contentType: MediaType? = null
    ) : DaoModelBuilder<File> {

        override fun build(): File {
            return File(
                    id = nonNull(id, "id"),
                    creationDate = nonNull(creationDate, "creationDate"),
                    modificationDate = nonNull(modificationDate, "modificationDate"),
                    name = nonNull(name, "name"),
                    owner = nonNull(owner, "owner"),
                    contentType = nonNull(contentType, "contentType")
            )
        }

        override fun getModelClass(): Class<File> {
            return File::class.java
        }
    }

    override fun toBuilder(): Builder {
        return Builder(
                id = id,
                creationDate = creationDate,
                modificationDate = modificationDate,
                name = name,
                owner = owner,
                contentType = contentType
        )
    }
}