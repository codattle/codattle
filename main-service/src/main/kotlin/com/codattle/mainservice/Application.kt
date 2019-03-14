package com.codattle.mainservice

import io.micronaut.runtime.Micronaut

object Application {

    @JvmStatic
    fun main(args: Array<String>) {
        Micronaut.build()
                .packages("com.codattle.mainservice")
                .mainClass(Application.javaClass)
                .start()
    }
}