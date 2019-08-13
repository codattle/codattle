package com.codattle.core.service.storage

import io.micronaut.context.annotation.Requires
import io.micronaut.context.annotation.Value
import java.io.File
import java.io.InputStream
import javax.inject.Singleton

@Singleton
@Requires(notEnv = ["prod"])
class LocalStorageService(
        @Value("\${codattle.storage.local.path:../develop/storage}") private val path: String
) : StorageService {

    override fun upload(name: String, data: InputStream) {
        File(path, name).apply { parentFile.mkdirs() }.writeBytes(data.readBytes())
    }
}