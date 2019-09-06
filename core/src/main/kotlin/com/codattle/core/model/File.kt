package com.codattle.core.model

import com.codattle.annotation.processing.annotation.GenerateDaoModelBuilder
import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils.nonNull
import io.micronaut.http.MediaType
import java.time.Instant

@GenerateDaoModelBuilder
data class File(
        override val id: Id<File>,
        override val creationDate: Instant,
        val name: String,
        val owner: Id<User>,
        val contentType: MediaType
) : DaoModel<File> {

    companion object {
        fun builder(): FileBuilder {
            return FileBuilder.withoutDefault()
        }
    }

    override fun toBuilder(): FileBuilder {
        return FileBuilder.fromModel(this)
    }
}