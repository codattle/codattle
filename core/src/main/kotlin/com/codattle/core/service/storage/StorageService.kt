package com.codattle.core.service.storage

import java.io.InputStream

interface StorageService {

    fun upload(name: String, data: InputStream)

    fun delete(name: String)

}