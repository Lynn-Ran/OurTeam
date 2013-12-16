#ifndef STATE_PROXY_H
#define STATE_PROXY_H

/* TODO: modify corresponding includes please */
#include "../example/src/lua.hpp"
#include "../example/src/lstate.h"


/*
** interface
*/
namespace lua_proxy {

    typedef void (*err_handler) ( const char *errmsg );

    /*
    ** 初始化 proxy
    ** L1: 被代理 state (私有 state)
    ** L2: 代理 state (公有 state)
    ** handler: 代理调用时的错误处理函数
    ** topmod: 模块名，注册的代理函数全部在该模块下，默认为 proxy
    ** return: 成功返回 true
    */
    bool InitProxy ( lua_State *L1, lua_State *L2, err_handler handler, const char *topmod = NULL );

    /*
    ** 添加一个代理函数
    ** proxy: 代理函数名
    ** origin: 原始函数名
    ** mod: 子模块名，建议同组函数归属同一模块
    ** return: 添加成功返回 true
    */
    bool AddProxy ( const char *proxy, const char *origin, const char *mod = NULL );

    /*
    ** 删除一个代理函数
    ** proxy: 代理函数名
    ** mod: 归属的子模块名，默认不归属子模块
    ** return: 删除则返回 true
    */
    bool DelPorxy ( const char *proxy, const char *mod = NULL );

    /*
    ** 获取错误信息
    */
    const char * GetErrMsg ();

} // end of namespace lua_proxy

#endif