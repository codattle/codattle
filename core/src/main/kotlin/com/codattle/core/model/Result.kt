package com.codattle.core.model

import dev.morphia.annotations.Embedded

@Embedded
class Result {

    var winner: Int? = null

    var resultFrames: List<ResultFrame> = listOf()

}