package com.codattle.core.service

import com.codattle.core.dao.UserDao
import com.codattle.core.dao.common.Id
import com.codattle.core.model.User
import javax.inject.Singleton

@Singleton
class UserService(private val userDao: UserDao) {

    fun getUser(userId: Id<User>): User {
        return userDao.getUser(userId)
    }
}