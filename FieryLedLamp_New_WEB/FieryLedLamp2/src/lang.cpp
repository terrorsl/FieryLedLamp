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
    supports[RUSSIAN].name=GetName(RUSSIAN);
    supports[RUSSIAN].effect_name=GetEffect(RUSSIAN);
#endif
};
void Languages::setLanguage(LanguageType index)
{
    if(index>=MAX_LANGUAGE)
        return;
    current=&supports[index];
};