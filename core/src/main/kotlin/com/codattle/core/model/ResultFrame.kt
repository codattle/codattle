package com.codattle.core.model

import dev.morphia.annotations.Embedded
import org.bson.Document

@Embedded
class ResultFrame {

    // 0-based order number
    var order: Int = 0

    var content: Document? = null
}