package com.codattle.core.model

class I18nText() : HashMap<Language, String>() {

    constructor(language: Language, text: String) : this() {
        put(language, text)
    }
}