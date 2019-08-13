package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.File
import javax.inject.Singleton

@Singleton
class FileDao(private val daoUtils: DaoUtils) {

    fun createFile(file: File.Builder): File {
        return daoUtils.save(file)
    }

    fun removeFile(id: Id<File>) {
        daoUtils.remove(id, File::class.java)
    }
}