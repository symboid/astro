
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_INIT_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_INIT_H__

#include "astro/uicontrols-qt/defs.h"
#include "sdk/arch/modqt.h"
#include "sdk/uicontrols-qt/init.h"
#include "sdk/network/init.h"
#include "sdk/hosting/init.h"
#include "astro/uicontrols-qt/arccoord.h"
#include "astro/uicontrols-qt/qastrofont.h"

struct ASTRO_UICONTROLS_QT_API mod_astro_uicontrols_qt : arh::mod_qt<mod_astro_uicontrols_qt>
{
    MOD_OBJECT(astro_uicontrols_qt)

    static constexpr const char* qml_pkg_name = "Symboid.Astro.Controls";
    static constexpr int qml_pkg_ver_major = 1;
    static constexpr int qml_pkg_ver_minor = 0;

    mod_astro_uicontrols_qt();
    ~mod_astro_uicontrols_qt();

    arh::mod_init<mod_sdk_network> _M_mod_sdk_network;
    arh::mod_init<mod_sdk_uicontrols_qt> _M_mod_sdk_uicontrols_qt;
    arh::mod_init<mod_sdk_hosting> _M_mod_sdk_hosting;

    qml_type_register<QArcCoord> _reg_arc_coord;
    qml_type_register<QNoneSectionCalc> _none_section_calc;
    qml_type_register<QZodiacSectionCalc> _zodiac_section_calc;
    qml_type_register<QSignumSectionCalc> _signum_section_calc;
    arh::main_object_init<QAstroFontRepo> _M_astro_font_repo;
};


#endif // __SYMBOID_ASTRO_UICONTROLS_QT_INIT_H__
