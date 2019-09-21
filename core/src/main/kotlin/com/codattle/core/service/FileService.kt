package com.codattle.core.service

import com.codattle.core.dao.FileDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.File
import com.codattle.core.model.FileBuilder
import com.codattle.core.model.User
import com.codattle.core.service.storage.StorageService
import io.micronaut.http.MediaType
import java.io.InputStream
import javax.inject.Singleton

@Singleton
class FileService(private val storageService: StorageService, private val fileDao: FileDao) {

    fun getFile(fileId: Id<File>): File {
        return fileDao.getFile(fileId)
    }

    fun addImage(fileName: String, contentType: MediaType, data: InputStream, owner: Id<User>): File {
        val file = fileDao.saveFile(File.builder()
                .name(fileName)
                .contentType(contentType)
                .owner(owner)
        )

        try {
            storageService.upload(file.id.value, data)
            return file
        } catch (exception: Exception) {
            fileDao.removeFile(file.id)
            throw exception
        }
    }

    fun deleteFile(fileId: Id<File>) {
        storageService.delete(fileId.value)
    }

}