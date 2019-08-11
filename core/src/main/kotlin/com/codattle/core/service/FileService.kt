package com.codattle.core.service

import com.codattle.core.dao.FileDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.File
import com.codattle.core.service.storage.StorageService
import io.micronaut.http.MediaType
import java.io.InputStream
import javax.inject.Singleton

@Singleton
class FileService(private val storageService: StorageService, private val fileDao: FileDao) {

    fun addImage(fileName: String, contentType: MediaType, data: InputStream): File {
        val file = fileDao.createFile(File.Builder(
                name = fileName,
                contentType = contentType,
                // TODO: pass real user after implementing users
                owner = Id("nonexistent_user")
        ))

        try {
            storageService.upload(file.id.value, data)
            return file
        } catch (exception: Exception) {
            fileDao.removeFile(file.id)
            throw exception
        }
    }
}