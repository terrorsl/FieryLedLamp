#ifndef LANG_FILE
#define LANG_FILE

#define SUPPORT_ENGLISH
#define SUPPORT_RUSSIAN

#if defined(SUPPORT_ENGLISH)
#include"lang_en.h"
#endif
#if defined(SUPPORT_RUSSIAN)
#include"lang_ru.h"
#endif

typedef enum{
#ifdef SUPPORT_ENGLISH
    ENGLISH,
#endif
#ifdef SUPPORT_RUSSIAN
    RUSSIAN,
#endif    
    MAX_LANGUAGE
}LanguageType;

struct Language
{
    const char *name;
    const char **effect_name;
    const char **template_manes;
};

class Languages
{
public:
    Languages();
    void setLanguage(LanguageType index);

    const char *GetName(){return current->name;}
    const char *GetEffect(unsigned short index){return current->effect_name[index];}
    const char *GetTemplateName(int index){return current->template_manes[index];}
private:
    Language supports[MAX_LANGUAGE];
    Language *current;
};
#endif