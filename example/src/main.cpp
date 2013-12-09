#include <stdio.h>
#include <stdlib.h>
#include "../../src/proxy.h"

using namespace lua_proxy;

void handle_err ( const char *errmsg ) {
    printf( "%s\n", errmsg );
}

static int max ( lua_State *L ) {
    int a = lua_tointeger( L, -1 );
    int b = lua_tointeger( L, -2 );
    lua_pushinteger( L, a >= b ? a : b );
    return 1;
}


int main () {
    
    lua_State *L1 = luaL_newstate();
    lua_State *L2 = luaL_newstate();
    luaL_openlibs( L1 );
    luaL_openlibs( L2 );

    lua_register( L1, "getmax", max );

    InitProxy( L1, L2, handle_err, "plugin" );

    AddProxy( "max", "getmax", "math" );

    lua_getglobal( L2, "plugin" );
    lua_getfield( L2, -1, "math" );
    lua_getfield( L2, -1, "max" );
    lua_pushinteger( L2, 5 );
    lua_pushinteger( L2, 10 );
    if ( lua_pcall( L2, 2, 1, 0 ) ) {
        printf( "%s\n", lua_tostring( L2, -1 ) );
        lua_pop( L2, -1 );
    } else {
        printf( "%d\n", lua_tointeger( L2, -1 ) );
        lua_pop( L2, -1 );
    }

    lua_close( L1 );
    lua_close( L2 );
    system( "pause" );
    return 0;
}