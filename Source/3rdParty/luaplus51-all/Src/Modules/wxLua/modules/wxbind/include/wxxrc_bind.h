// ---------------------------------------------------------------------------
// wxxrc.h - headers and wxLua types for wxLua binding
//
// This file was generated by genwxbind.lua 
// Any changes made to this file will be lost when the file is regenerated
// ---------------------------------------------------------------------------

#ifndef __HOOK_WXLUA_wxxrc_H__
#define __HOOK_WXLUA_wxxrc_H__

#include "wxbind/include/wxbinddefs.h"
#include "wxluasetup.h"
#include "wxbind/include/wxcore_bind.h"

#include "wxlua/include/wxlstate.h"
#include "wxlua/include/wxlbind.h"

// ---------------------------------------------------------------------------
// Check if the version of binding generator used to create this is older than
//   the current version of the bindings.
//   See 'bindings/genwxbind.lua' and 'modules/wxlua/include/wxldefs.h'
#if WXLUA_BINDING_VERSION > 30
#   error "The WXLUA_BINDING_VERSION in the bindings is too old, regenerate bindings."
#endif //WXLUA_BINDING_VERSION > 30
// ---------------------------------------------------------------------------

// binding class
class WXDLLIMPEXP_BINDWXXRC wxLuaBinding_wxxrc : public wxLuaBinding
{
public:
    wxLuaBinding_wxxrc();


private:
    DECLARE_DYNAMIC_CLASS(wxLuaBinding_wxxrc)
};


// initialize wxLuaBinding_wxxrc for all wxLuaStates
extern WXDLLIMPEXP_BINDWXXRC wxLuaBinding* wxLuaBinding_wxxrc_init();

// ---------------------------------------------------------------------------
// Includes
// ---------------------------------------------------------------------------

#if wxLUA_USE_wxXRC && wxUSE_XRC
    #include "wx/xrc/xmlres.h"
#endif // wxLUA_USE_wxXRC && wxUSE_XRC

// ---------------------------------------------------------------------------
// Lua Tag Method Values and Tables for each Class
// ---------------------------------------------------------------------------

#if wxLUA_USE_wxXRC && wxUSE_XRC
    extern WXDLLIMPEXP_DATA_BINDWXXRC(int) wxluatype_wxXmlResource;
#endif // wxLUA_USE_wxXRC && wxUSE_XRC



#endif // __HOOK_WXLUA_wxxrc_H__

