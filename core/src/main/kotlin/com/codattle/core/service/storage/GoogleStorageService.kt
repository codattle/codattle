package com.codattle.core.service.storage

import com.google.auth.oauth2.ServiceAccountCredentials
import com.google.cloud.storage.BlobId
import com.google.cloud.storage.BlobInfo
import com.google.cloud.storage.Storage
import com.google.cloud.storage.StorageOptions
import io.micronaut.context.annotation.Requires
import io.micronaut.context.annotation.Value
import java.io.InputStream
import java.util.*
import javax.inject.Singleton

@Singleton
@Requires(env = ["prod"])
class GoogleStorageService(
        @Value("\${codattle.storage.google.credentials}") private val credentials: String,
        @Value("\${codattle.storage.google.bucket.name}") private val bucketName: String
) : StorageService {

    private val storage: Storage = StorageOptions.newBuilder()
            .setCredentials(ServiceAccountCredentials.fromStream(credentials.byteInputStream()))
            .build()
            .service

    override fun upload(name: String, data: InputStream) {
        storage.create(BlobInfo.newBuilder(BlobId.of(bucketName, name)).build(), data.readBytes())
    }

    override fun delete(name: String) {
        storage.delete(BlobId.of(bucketName, name))
    }

}