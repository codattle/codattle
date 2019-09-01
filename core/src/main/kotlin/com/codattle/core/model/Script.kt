package com.codattle.core.model

import com.codattle.annotation.processing.annotation.GenerateDaoModelBuilder
import com.codattle.core.dao.common.DaoModel
import com.codattle.core.dao.common.DaoModelBuilder
import com.codattle.core.dao.common.Id
import com.codattle.core.model.utils.ModelUtils.nonNull
import java.time.Instant

@GenerateDaoModelBuilder
data class Script(
        override val id: Id<Script>,
        override val creationDate: Instant,
        val game: Id<Game>,
        val code: String,
        val author: Id<User>,
        val visibility: Visibility
) : DaoModel<Script> {

    enum class Visibility {
        PUBLIC, PRIVATE
    }

    companion object {
        fun builder(): ScriptBuilder {
            return ScriptBuilder.withoutDefault(visibility = Visibility.PRIVATE)
        }
    }

    override fun toBuilder(): ScriptBuilder {
        return ScriptBuilder.fromModel(this)
    }
}

