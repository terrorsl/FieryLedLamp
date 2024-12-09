#include"lang.h"
#include"effect.h"

#define GetName(lang) lang##_NAME
#define GetEffect(lang) lang##_EFFECT
#define GetTemplateName(lang) lang##_TEMPLATE_NAME
#define GetArraySize(arr) (sizeof(arr)/sizeof(arr[0]))

#define ASSERT_ARRAY(COND, _name) typedef char static_assertion_##_name[(!!(COND))*2-1]
#define ASSERT_ARRAY2(lang) ASSERT_ARRAY(GetArraySize(GetEffect(lang))==FieryLedLampEffectTypes::MaxEffect, lang##_EFFECT) __attribute__((unused))
 
#define INITIALISE_LANGUAGE(lang) supports[lang].name = GetName(lang);\
    supports[lang].effect_name=GetEffect(lang);\
    supports[lang].template_manes=GetTemplateName(lang);\
    ASSERT_ARRAY2(lang)

Languages::Languages()
{
#ifdef SUPPORT_ENGLISH
    INITIALISE_LANGUAGE(ENGLISH);
#endif
#ifdef SUPPORT_RUSSIAN
    INITIALISE_LANGUAGE(RUSSIAN);
#endif
};
void Languages::setLanguage(LanguageType index)
{
    if(index>=MAX_LANGUAGE)
        return;
    current=&supports[index];
};