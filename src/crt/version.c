/**
 * version.c — XDK/locale bootstrap check
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * _check_xdk_version validates executable privilege and two XConfig gates,
 * then selects and registers localized CRT strings based on XGetLanguage().
 *
 * _check_xdk_version @ 0x8242BB70 | size: 0x1A4
 */

#include <stddef.h>
#include <stdint.h>

/* External helpers imported from the XEX/kernel layer */
uint32_t xe_XexCheckEcutablePrivilege_5DBC(uint32_t privilege);
int ExGetXConfigSetting(uint32_t category, uint32_t setting, uint32_t* value, uint32_t valueSize, uint32_t* outSize);
uint32_t XGetLanguage(void);
uint16_t* _locale_lookup(uint32_t localeId);
void _locale_merge(uint16_t* primary, uint16_t* secondary, int mergeMode);
void _locale_register(void* localeString, int registrationType);

/**
 * _check_xdk_version @ 0x8242BB70
 */
int _check_xdk_version(void)
{
    uint32_t configSize = 0;
    uint32_t settingA = 0;
    uint32_t settingB = 0;

    static const uint32_t kPrimaryLocaleMap[10] = {
        9, 9, 13, 11, 10, 16, 12, 14, 17, 15
    };

    static const uint32_t kSecondaryLocaleMap[10] = {
        0, 0, 4, 2, 1, 7, 3, 5, 8, 6
    };

    if (xe_XexCheckEcutablePrivilege_5DBC(10) == 0) {
        return 0;
    }

    if (ExGetXConfigSetting(2, 2, &settingA, 4, &configSize) != 0) {
        return 0;
    }

    if ((settingA & 0x0000FF00u) != 0x00000300u) {
        return 0;
    }

    if (ExGetXConfigSetting(3, 10, &settingB, 4, &configSize) != 0) {
        return 0;
    }

    if (((settingB & 0x00800000u) == 0u) && ((settingB & 0x00400000u) != 0u)) {
        return 0;
    }

    uint32_t language = XGetLanguage();
    if (language >= 10u) {
        language = 1u;
    }

    uint16_t* primaryLocale = _locale_lookup(kPrimaryLocaleMap[language]);
    if (primaryLocale == NULL) {
        return 0;
    }

    uint16_t* secondaryLocale = _locale_lookup(kSecondaryLocaleMap[language]);
    if (secondaryLocale != NULL) {
        _locale_merge(primaryLocale, secondaryLocale, 1);
        _locale_register(secondaryLocale, 0x24830000);
    }

    _locale_register(primaryLocale, 0x24830000);
    return 1;
}
