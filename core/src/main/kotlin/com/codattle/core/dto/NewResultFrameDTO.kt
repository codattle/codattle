package com.codattle.core.dto

import com.codattle.core.model.ResultFrame
import org.bson.Document

data class NewResultFrameDTO(val order: Int, val content: String) {

    fun toModel(): ResultFrame {
        val model = ResultFrame()
        model.order = order
        model.content = Document.parse(content)
        return model
    }
}