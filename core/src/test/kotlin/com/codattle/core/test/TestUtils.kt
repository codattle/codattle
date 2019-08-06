package com.codattle.core.test

import com.codattle.core.dao.common.Id
import com.codattle.core.model.Game
import com.codattle.core.model.Match

object TestUtils {

    val nonexistentGameId = Id<Game>("1000000")
    val nonexistentMatchId = Id<Match>("1000000")
}



