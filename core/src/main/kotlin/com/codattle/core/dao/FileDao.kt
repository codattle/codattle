package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.File
import com.codattle.core.model.Game
import javax.inject.Singleton

@Singleton
class FileDao(private val daoUtils: DaoUtils) {

    fun getFile(fileId: Id<File>): File {
        return daoUtils.get(fileId)
                ?: throw IllegalArgumentException("File with id \"$fileId\" doesn't exist.")
    }

    fun saveFile(file: File.Builder): File {
        return daoUtils.save(file)
    }

    fun removeFile(id: Id<File>) {
        daoUtils.remove(id, File::class.java)
    }
}