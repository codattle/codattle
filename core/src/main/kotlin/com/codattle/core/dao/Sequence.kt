package com.codattle.core.dao

import com.mongodb.client.model.Filters
import com.mongodb.client.model.FindOneAndUpdateOptions
import com.mongodb.client.model.ReturnDocument
import com.mongodb.client.model.Updates
import javax.inject.Singleton

@Singleton
class Sequence(private val mongoProvider: MongoProvider) {

    companion object {
        private const val SEQUENCE_COLLECTION_NAME = "sequence"
        private const val KEY = "key"
        private const val SEQ = "seq"
    }

    fun getNext(key: String): Int {
        val sequence = mongoProvider.database.getCollection(SEQUENCE_COLLECTION_NAME).findOneAndUpdate(
                Filters.eq(KEY, key),
                Updates.inc(SEQ, 1),
                FindOneAndUpdateOptions().upsert(true).returnDocument(ReturnDocument.AFTER))
        return sequence!!.getInteger(SEQ)
    }
}