package com.codattle.core.model

data class MatchResult(val resultFrames: List<ResultFrame> = listOf(), val winner: Int? = null)