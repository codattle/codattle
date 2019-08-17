package com.codattle.core.model

import com.codattle.core.dao.common.Id

data class Sprite(val name: String, val image: Id<File>)