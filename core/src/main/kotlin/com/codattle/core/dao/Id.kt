package com.codattle.core.dao

data class Id<T : DaoModel<T>>(val id: String) {

    private interface UncheckedDaoModel : DaoModel<UncheckedDaoModel>

    companion object {
        fun unchecked(id: String): Id<*> {
            return Id<UncheckedDaoModel>(id)
        }
    }

    override fun toString(): String {
        return id
    }
}