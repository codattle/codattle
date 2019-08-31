package com.codattle.core.dao

import com.codattle.core.dao.common.DaoUtils
import com.codattle.core.dao.common.Id
import com.codattle.core.model.User
import javax.inject.Singleton

@Singleton
class UserDao(private val daoUtils: DaoUtils) {

    fun getUser(userId: Id<User>): User {
        return daoUtils.get(userId) ?: throw IllegalArgumentException("User with id \"$userId\" doesn't exist.")
    }
}