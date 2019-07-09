package com.codattle.core.dao

class Id<T : DaoModel<T>>(val id: Int) {

    private class UncheckedDaoModel : DaoModel<UncheckedDaoModel>()

    companion object {
        fun unchecked(id: Int): Id<*> {
            return Id<UncheckedDaoModel>(id)
        }
    }

    override fun toString(): String {
        return id.toString()
    }
}