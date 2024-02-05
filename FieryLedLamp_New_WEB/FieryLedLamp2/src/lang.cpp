#include"lang.h"

#define GetName(lang) lang##_NAME
#define GetEffect(lang) lang##_EFFECT

#define APPEND_LANGUAGE(lang) supports[lang].name=

Languages::Languages()
{
#ifdef SUPPORT_ENGLISH
    #include"lang_en.h"
    supports[ENGLISH].name=GetName(ENGLISH);
#endif
#ifdef SUPPORT_RUSSIAN
    #include"lang_ru.h"
    supports[RUSSIAN].name=RUSSIAN_NAME;
    supports[RUSSIAN].effect_name=RUSSIAN_EFFECT;
#endif
};
void Languages::setLanguage(LanguageType index)
{
    if(index>=MAX_LANGUAGE)
        return;
    current=&supports[index];
};