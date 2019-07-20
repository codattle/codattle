package com.codattle.core.service

import com.codattle.core.test.BaseTest
import org.assertj.core.api.Assertions.assertThat
import java.util.*
import javax.inject.Inject

class QueueServiceTest : BaseTest() {

    @Inject
    private lateinit var queueService: QueueService

    fun `create queue`() {
        val queueName = "MY_QUEUE-${UUID.randomUUID()}"
        queueService.createQueue(queueName)

        assertThat(queueHelper.doesQueueExist(queueName)).isTrue()
    }

    fun `send message to existed queue`() {
        val queueName = "MY_QUEUE-${UUID.randomUUID()}"
        queueService.createQueue(queueName)
        val message = ByteArray(10) { (it * 3).toByte() }

        queueService.sendMessage(queueName, message)

        assertThat(queueHelper.countMessages(queueName)).isEqualTo(1)
        assertThat(queueHelper.readMessage(queueName)).isEqualTo(message)
    }
}