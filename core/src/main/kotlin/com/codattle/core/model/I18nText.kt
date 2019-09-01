package com.codattle.core.model

class I18nText : HashMap<Language, String>() {

    companion object {
        fun single(language: Language, content: String): I18nText {
            return I18nText().apply { put(language, content) }
        }
    }

    fun toListOfTranslatedTexts(): List<TranslatedText> {
        return map { TranslatedText(it.key, it.value) }.toList()
    }
}