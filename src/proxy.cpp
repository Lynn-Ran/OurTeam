#include "proxy.h"



/*
** helpers
*/
namespace {

#define PROXY_OK                  1
#define PROXY_ERROR               2
#define PROXY_INVALID_ARGS        3
#define PROXY_INVALID_MOD         4
#define PROXY_ORIGIN_NOT_FUNC     5
#define PROXY_INVOKE_ERR          6
    
    lua_State                   *   Li;
    lua_State                   *   Lo;
    char                        *   proxy_module;
    lua_proxy::err_handler          handler_fn;
    int                             errcode;

    int proxy_fn ( lua_State *L ) {
        int n = L->top - L->ci->func - 1;
        int r = L->ci->nresults;
        lua_getglobal( Li, lua_tostring( L, lua_upvalueindex( 1 ) ) );

        lua_xmove( L, Li, n );
        if ( lua_pcall( Li, n, r, 0 ) ) {
            if ( NULL != handler_fn ) {
                handler_fn( lua_tostring( Li, -1 ) );
            }
            lua_pop( Li, -1 );
            return 0;
        }
        lua_xmove( Li, L, r );

        return r;
    }

} // end of anonymous namespace


/*
** interface
*/
namespace lua_proxy {

    bool InitProxy ( lua_State *L1, lua_State *L2, err_handler handler, const char *topmod ) {
        if ( NULL == L1 || NULL == L2 ) {
            errcode = PROXY_INVALID_ARGS;
            return false;
        }

        Li              = L1;
        Lo              = L2;
        handler_fn      = handler;
        proxy_module    = topmod ? topmod : "proxy";

        lua_newtable( Lo );
        lua_setglobal( Lo, proxy_module );
    }


    bool AddProxy ( const char *proxy, const char *origin, const char *mod ) {
        if ( NULL == proxy || NULL == origin ) {
            errcode = PROXY_INVALID_ARGS;
            return false;
        }

        lua_getglobal( Li, origin );
        if ( !lua_isfunction( Li, -1 ) ) {
            lua_pop( Li, 1 );
            errcode = PROXY_ORIGIN_NOT_FUNC;
            return false;
        }
        lua_pop( Li, 1 );

        lua_getglobal( Lo, proxy_module );

        if ( NULL != mod ) {
            lua_getfield( Lo, -1, mod );
            if ( lua_isnil( Lo, -1 ) ) {
                lua_pop( Lo, 1 );
                lua_newtable( Lo );
                lua_setfield( Lo, -2, mod );
                lua_getfield( Lo, -1, mod );
            } else if ( !lua_istable( Lo, -1 ) ) {
                lua_pop( Lo, 2 );
                errcode = PROXY_INVALID_MOD;
                return false;
            }
        }

        lua_pushstring( Lo, origin );
        lua_pushcclosure( Lo, proxy_fn, 1 );
        lua_setfield( Lo, -2, proxy );
        lua_pop( Lo, mod ? 2 : 1 );

        return true;
    }


    bool DelPorxy ( const char *proxy, const char *mod ) {
        if ( NULL == proxy ) {
            errcode = PROXY_INVALID_ARGS;
            return false;
        }

        lua_getglobal( Lo, proxy_module );

        if ( NULL != mod ) {
            lua_getfield( Lo, -1, mod );
            if ( !lua_istable( Lo, -1 ) ) {
                lua_pop( Lo, 2 );
                errcode = PROXY_INVALID_MOD;
                return false;
            }
            lua_pushnil( Lo );
            lua_setfield( Lo, -2, proxy );
            lua_pop( Lo, 2 );
            return true;
        } else {
            lua_pushnil( Lo );
            lua_setfield( Lo, -2, proxy );
            lua_pop( Lo, 1 );
            return true;
        }
    }


    const char * GetErrMsg () {
        switch ( errcode ) {
            case PROXY_OK               : return "ok";
            case PROXY_ERROR            : return "error";
            case PROXY_INVALID_ARGS     : return "invalid arguments";
            case PROXY_INVALID_MOD      : return "invalid module";
            case PROXY_ORIGIN_NOT_FUNC  : return "origin is not a function";
            case PROXY_INVOKE_ERR       : return "invoke failed in private state";
            default                     : return "error not in handle";
        }
    }
} // end of namespace lua_proxy