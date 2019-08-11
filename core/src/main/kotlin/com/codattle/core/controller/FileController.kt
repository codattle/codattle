package com.codattle.core.controller

import com.codattle.core.service.FileService
import io.micronaut.http.MediaType
import io.micronaut.http.annotation.Consumes
import io.micronaut.http.annotation.Controller
import io.micronaut.http.annotation.Post
import io.micronaut.http.annotation.Produces
import io.micronaut.http.multipart.CompletedFileUpload

@Controller("/api/v1/file")
class FileController(private val fileService: FileService) {

    @Post
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces(MediaType.APPLICATION_JSON)
    fun sendImage(file: CompletedFileUpload): Map<String, Any> {
        val contentType = file.contentType.orElseThrow { IllegalArgumentException("No content type") }
        val image = fileService.addImage(file.name, contentType, file.inputStream)
        return mapOf("id" to image.id.value)
    }
}