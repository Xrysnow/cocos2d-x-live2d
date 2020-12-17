#include "lua_live2d_auto.hpp"
#include "L2DModel.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#ifndef COCOS2D_DEBUG
#define COCOS2D_DEBUG 0
#endif

int lua_L2D_Model_addParameter(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_addParameter'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 2) 
    {
        std::string arg0;
        double arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:addParameter");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:addParameter");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_addParameter'", nullptr);
            return 0;
        }
        cobj->addParameter(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:addParameter");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:addParameter");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:addParameter");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_addParameter'", nullptr);
            return 0;
        }
        cobj->addParameter(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:addParameter", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_addParameter'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_areaHitTest(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_areaHitTest'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 3) 
    {
        const char* arg0;
        double arg1;
        double arg2;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "l2d.Model:areaHitTest"); arg0 = arg0_tmp.c_str();

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:areaHitTest");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:areaHitTest");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_areaHitTest'", nullptr);
            return 0;
        }
        bool ret = cobj->areaHitTest(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:areaHitTest", argc, 3);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_areaHitTest'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:create");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "l2d.Model:create");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_L2D_Model_create'", nullptr);
            return 0;
        }
        l2d::Model* ret = l2d::Model::create(arg0, arg1);
        object_to_luaval<l2d::Model>(tolua_S, "l2d.Model", (l2d::Model*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "l2d.Model:create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_create'.",&tolua_err);
#endif
    return 0;
}
int lua_L2D_Model_getCanvasOrigin(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getCanvasOrigin'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getCanvasOrigin'", nullptr);
            return 0;
        }
        cocos2d::Vec2 ret = cobj->getCanvasOrigin();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getCanvasOrigin", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getCanvasOrigin'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getCanvasSize(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getCanvasSize'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getCanvasSize'", nullptr);
            return 0;
        }
        cocos2d::Size ret = cobj->getCanvasSize();
        size_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getCanvasSize", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getCanvasSize'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getDebugRectRenderer(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getDebugRectRenderer'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getDebugRectRenderer'", nullptr);
            return 0;
        }
        cocos2d::DrawNode* ret = cobj->getDebugRectRenderer();
        object_to_luaval<cocos2d::DrawNode>(tolua_S, "cc.DrawNode", (cocos2d::DrawNode*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDebugRectRenderer", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getDebugRectRenderer'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getDirectory(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getDirectory'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getDirectory'", nullptr);
            return 0;
        }
        std::string ret = cobj->getDirectory();
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDirectory", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getDirectory'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getDrawableBlendMode(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getDrawableBlendMode'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getDrawableBlendMode");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getDrawableBlendMode'", nullptr);
            return 0;
        }
        int ret = cobj->getDrawableBlendMode(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableBlendMode", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getDrawableBlendMode'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getDrawableCount(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getDrawableCount'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getDrawableCount'", nullptr);
            return 0;
        }
        int ret = cobj->getDrawableCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableCount", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getDrawableCount'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getDrawableCulling(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getDrawableCulling'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getDrawableCulling");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getDrawableCulling'", nullptr);
            return 0;
        }
        int ret = cobj->getDrawableCulling(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableCulling", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getDrawableCulling'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getDrawableNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getDrawableNames'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getDrawableNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getDrawableNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableNames", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getDrawableNames'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getDrawableOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getDrawableOpacity'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getDrawableOpacity");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getDrawableOpacity'", nullptr);
            return 0;
        }
        double ret = cobj->getDrawableOpacity(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableOpacity", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getDrawableOpacity'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getExpressionNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getExpressionNames'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getExpressionNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getExpressionNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getExpressionNames", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getExpressionNames'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getGravity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getGravity'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getGravity'", nullptr);
            return 0;
        }
        cocos2d::Vec2 ret = cobj->getGravity();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getGravity", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getGravity'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getLipSync(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getLipSync'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getLipSync'", nullptr);
            return 0;
        }
        bool ret = cobj->getLipSync();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getLipSync", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getLipSync'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getLipValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getLipValue'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getLipValue'", nullptr);
            return 0;
        }
        double ret = cobj->getLipValue();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getLipValue", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getLipValue'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getMocVersion(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getMocVersion'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getMocVersion'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->getMocVersion();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getMocVersion", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getMocVersion'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getModelColor(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getModelColor'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getModelColor'", nullptr);
            return 0;
        }
        cocos2d::Color4B ret = cobj->getModelColor();
        color4b_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getModelColor", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getModelColor'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getModelOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getModelOpacity'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getModelOpacity'", nullptr);
            return 0;
        }
        uint16_t ret = cobj->getModelOpacity();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getModelOpacity", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getModelOpacity'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getMotionCount(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getMotionCount'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getMotionCount");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getMotionCount'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->getMotionCount(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getMotionCount", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getMotionCount'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getMotionGroupNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getMotionGroupNames'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getMotionGroupNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getMotionGroupNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getMotionGroupNames", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getMotionGroupNames'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getParameter(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getParameter'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getParameter");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getParameter'", nullptr);
            return 0;
        }
        double ret = cobj->getParameter(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameter", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getParameter'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getParameterCount(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getParameterCount'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getParameterCount'", nullptr);
            return 0;
        }
        int ret = cobj->getParameterCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterCount", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getParameterCount'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getParameterDefault(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getParameterDefault'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getParameterDefault");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getParameterDefault'", nullptr);
            return 0;
        }
        double ret = cobj->getParameterDefault(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterDefault", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getParameterDefault'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getParameterMax(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getParameterMax'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getParameterMax");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getParameterMax'", nullptr);
            return 0;
        }
        double ret = cobj->getParameterMax(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterMax", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getParameterMax'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getParameterMin(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getParameterMin'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getParameterMin");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getParameterMin'", nullptr);
            return 0;
        }
        double ret = cobj->getParameterMin(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterMin", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getParameterMin'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getParameterNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getParameterNames'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getParameterNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getParameterNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterNames", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getParameterNames'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getPartCount(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getPartCount'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getPartCount'", nullptr);
            return 0;
        }
        int ret = cobj->getPartCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getPartCount", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getPartCount'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getPartNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getPartNames'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getPartNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getPartNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getPartNames", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getPartNames'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getPartOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getPartOpacity'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getPartOpacity");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getPartOpacity'", nullptr);
            return 0;
        }
        double ret = cobj->getPartOpacity(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getPartOpacity", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getPartOpacity'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getPartParents(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getPartParents'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getPartParents'", nullptr);
            return 0;
        }
        std::vector<int> ret = cobj->getPartParents();
        ccvector_int_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getPartParents", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getPartParents'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getPixelsPerUnit(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getPixelsPerUnit'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getPixelsPerUnit'", nullptr);
            return 0;
        }
        double ret = cobj->getPixelsPerUnit();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getPixelsPerUnit", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getPixelsPerUnit'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getSoundFileName(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getSoundFileName'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 2) 
    {
        std::string arg0;
        int arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getSoundFileName");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "l2d.Model:getSoundFileName");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getSoundFileName'", nullptr);
            return 0;
        }
        std::string ret = cobj->getSoundFileName(arg0, arg1);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getSoundFileName", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getSoundFileName'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_getWind(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_getWind'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_getWind'", nullptr);
            return 0;
        }
        cocos2d::Vec2 ret = cobj->getWind();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getWind", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_getWind'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_multiplyParameter(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_multiplyParameter'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 2) 
    {
        std::string arg0;
        double arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:multiplyParameter");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:multiplyParameter");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_multiplyParameter'", nullptr);
            return 0;
        }
        cobj->multiplyParameter(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:multiplyParameter");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:multiplyParameter");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:multiplyParameter");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_multiplyParameter'", nullptr);
            return 0;
        }
        cobj->multiplyParameter(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:multiplyParameter", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_multiplyParameter'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_resetDragging(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_resetDragging'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_resetDragging'", nullptr);
            return 0;
        }
        cobj->resetDragging();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:resetDragging", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_resetDragging'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setAcceleration(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setAcceleration'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 3) 
    {
        double arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setAcceleration");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setAcceleration");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:setAcceleration");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setAcceleration'", nullptr);
            return 0;
        }
        cobj->setAcceleration(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setAcceleration", argc, 3);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setAcceleration'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setAutoDragging(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setAutoDragging'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "l2d.Model:setAutoDragging");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setAutoDragging'", nullptr);
            return 0;
        }
        cobj->setAutoDragging(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setAutoDragging", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setAutoDragging'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setBlinkingInterval(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setBlinkingInterval'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setBlinkingInterval");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setBlinkingInterval'", nullptr);
            return 0;
        }
        cobj->setBlinkingInterval(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setBlinkingInterval", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setBlinkingInterval'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setBlinkingSettings(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setBlinkingSettings'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 3) 
    {
        double arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setBlinkingSettings");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setBlinkingSettings");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:setBlinkingSettings");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setBlinkingSettings'", nullptr);
            return 0;
        }
        cobj->setBlinkingSettings(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setBlinkingSettings", argc, 3);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setBlinkingSettings'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setDebugRectEnable(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setDebugRectEnable'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "l2d.Model:setDebugRectEnable");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setDebugRectEnable'", nullptr);
            return 0;
        }
        cobj->setDebugRectEnable(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setDebugRectEnable", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setDebugRectEnable'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setDragging(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setDragging'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setDragging");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setDragging");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setDragging'", nullptr);
            return 0;
        }
        cobj->setDragging(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setDragging", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setDragging'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setExpression(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setExpression'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        const char* arg0;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "l2d.Model:setExpression"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setExpression'", nullptr);
            return 0;
        }
        bool ret = cobj->setExpression(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setExpression", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setExpression'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setGravity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setGravity'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        cocos2d::Vec2 arg0;

        ok &= luaval_to_vec2(tolua_S, 2, &arg0, "l2d.Model:setGravity");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setGravity'", nullptr);
            return 0;
        }
        cobj->setGravity(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setGravity", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setGravity'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setLipSync(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setLipSync'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "l2d.Model:setLipSync");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setLipSync'", nullptr);
            return 0;
        }
        cobj->setLipSync(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setLipSync", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setLipSync'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setLipValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setLipValue'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setLipValue");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setLipValue'", nullptr);
            return 0;
        }
        cobj->setLipValue(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setLipValue", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setLipValue'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setModelColor(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setModelColor'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        cocos2d::Color4B arg0;

        ok &=luaval_to_color4b(tolua_S, 2, &arg0, "l2d.Model:setModelColor");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setModelColor'", nullptr);
            return 0;
        }
        cobj->setModelColor(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setModelColor", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setModelColor'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setModelOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setModelOpacity'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        uint16_t arg0;

        ok &= luaval_to_uint16(tolua_S, 2,&arg0, "l2d.Model:setModelOpacity");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setModelOpacity'", nullptr);
            return 0;
        }
        cobj->setModelOpacity(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setModelOpacity", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setModelOpacity'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setParameter(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setParameter'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 2) 
    {
        std::string arg0;
        double arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:setParameter");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setParameter");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setParameter'", nullptr);
            return 0;
        }
        cobj->setParameter(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:setParameter");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setParameter");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:setParameter");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setParameter'", nullptr);
            return 0;
        }
        cobj->setParameter(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setParameter", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setParameter'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setPartOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setPartOpacity'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 2) 
    {
        std::string arg0;
        double arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:setPartOpacity");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setPartOpacity");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setPartOpacity'", nullptr);
            return 0;
        }
        cobj->setPartOpacity(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setPartOpacity", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setPartOpacity'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setRandomExpression(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setRandomExpression'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setRandomExpression'", nullptr);
            return 0;
        }
        bool ret = cobj->setRandomExpression();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setRandomExpression", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setRandomExpression'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setSoundEventHandler(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setSoundEventHandler'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        std::function<void (const std::string&, LAppModel::SoundEventType)> arg0;

        int handler2 = toluafix_ref_function(tolua_S, 2, 0);
		ok &= handler2 != 0;
		if(ok)
		{
			arg0 = [=](const std::string& param0, LAppModel::SoundEventType param1)
			{
				toluafix_get_function_by_refid(tolua_S, handler2);
				if (!lua_isfunction(tolua_S, -1))
				{
					lua_pop(tolua_S, 1);
					return;
				}
                lua_pushlstring(tolua_S,param0.c_str(),param0.size());
				tolua_pushnumber(tolua_S,(lua_Number)param1);
				lua_call(tolua_S, 2, 0);
			};
		}
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setSoundEventHandler'", nullptr);
            return 0;
        }
        cobj->setSoundEventHandler(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setSoundEventHandler", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setSoundEventHandler'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setTouchDragging(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setTouchDragging'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setTouchDragging");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setTouchDragging");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setTouchDragging'", nullptr);
            return 0;
        }
        cobj->setTouchDragging(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setTouchDragging", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setTouchDragging'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_setWind(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_setWind'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) 
    {
        cocos2d::Vec2 arg0;

        ok &= luaval_to_vec2(tolua_S, 2, &arg0, "l2d.Model:setWind");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_setWind'", nullptr);
            return 0;
        }
        cobj->setWind(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setWind", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_setWind'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_startMotion(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_startMotion'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 3) 
    {
        const char* arg0;
        int arg1;
        int arg2;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "l2d.Model:startMotion"); arg0 = arg0_tmp.c_str();

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "l2d.Model:startMotion");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "l2d.Model:startMotion");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_startMotion'", nullptr);
            return 0;
        }
        bool ret = cobj->startMotion(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:startMotion", argc, 3);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_startMotion'.",&tolua_err);
    return 0;
#endif
}
int lua_L2D_Model_startRandomMotion(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_L2D_Model_startRandomMotion'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    if (argc == 2) 
    {
        const char* arg0;
        int arg1;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "l2d.Model:startRandomMotion"); arg0 = arg0_tmp.c_str();

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "l2d.Model:startRandomMotion");
        if(!ok)
        {
            tolua_error(tolua_S, "invalid arguments in function 'lua_L2D_Model_startRandomMotion'", nullptr);
            return 0;
        }
        bool ret = cobj->startRandomMotion(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:startRandomMotion", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_L2D_Model_startRandomMotion'.",&tolua_err);
    return 0;
#endif
}
static int lua_L2D_Model_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Model)");
    return 0;
}

int lua_register_L2D_Model(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"l2d.Model");
    tolua_cclass(tolua_S,"Model","l2d.Model","ccui.Widget",nullptr);

    tolua_beginmodule(tolua_S,"Model");
        tolua_function(tolua_S,"addParameter",lua_L2D_Model_addParameter);
        tolua_function(tolua_S,"areaHitTest",lua_L2D_Model_areaHitTest);
        tolua_function(tolua_S,"getCanvasOrigin",lua_L2D_Model_getCanvasOrigin);
        tolua_function(tolua_S,"getCanvasSize",lua_L2D_Model_getCanvasSize);
        tolua_function(tolua_S,"getDebugRectRenderer",lua_L2D_Model_getDebugRectRenderer);
        tolua_function(tolua_S,"getDirectory",lua_L2D_Model_getDirectory);
        tolua_function(tolua_S,"getDrawableBlendMode",lua_L2D_Model_getDrawableBlendMode);
        tolua_function(tolua_S,"getDrawableCount",lua_L2D_Model_getDrawableCount);
        tolua_function(tolua_S,"getDrawableCulling",lua_L2D_Model_getDrawableCulling);
        tolua_function(tolua_S,"getDrawableNames",lua_L2D_Model_getDrawableNames);
        tolua_function(tolua_S,"getDrawableOpacity",lua_L2D_Model_getDrawableOpacity);
        tolua_function(tolua_S,"getExpressionNames",lua_L2D_Model_getExpressionNames);
        tolua_function(tolua_S,"getGravity",lua_L2D_Model_getGravity);
        tolua_function(tolua_S,"getLipSync",lua_L2D_Model_getLipSync);
        tolua_function(tolua_S,"getLipValue",lua_L2D_Model_getLipValue);
        tolua_function(tolua_S,"getMocVersion",lua_L2D_Model_getMocVersion);
        tolua_function(tolua_S,"getModelColor",lua_L2D_Model_getModelColor);
        tolua_function(tolua_S,"getModelOpacity",lua_L2D_Model_getModelOpacity);
        tolua_function(tolua_S,"getMotionCount",lua_L2D_Model_getMotionCount);
        tolua_function(tolua_S,"getMotionGroupNames",lua_L2D_Model_getMotionGroupNames);
        tolua_function(tolua_S,"getParameter",lua_L2D_Model_getParameter);
        tolua_function(tolua_S,"getParameterCount",lua_L2D_Model_getParameterCount);
        tolua_function(tolua_S,"getParameterDefault",lua_L2D_Model_getParameterDefault);
        tolua_function(tolua_S,"getParameterMax",lua_L2D_Model_getParameterMax);
        tolua_function(tolua_S,"getParameterMin",lua_L2D_Model_getParameterMin);
        tolua_function(tolua_S,"getParameterNames",lua_L2D_Model_getParameterNames);
        tolua_function(tolua_S,"getPartCount",lua_L2D_Model_getPartCount);
        tolua_function(tolua_S,"getPartNames",lua_L2D_Model_getPartNames);
        tolua_function(tolua_S,"getPartOpacity",lua_L2D_Model_getPartOpacity);
        tolua_function(tolua_S,"getPartParents",lua_L2D_Model_getPartParents);
        tolua_function(tolua_S,"getPixelsPerUnit",lua_L2D_Model_getPixelsPerUnit);
        tolua_function(tolua_S,"getSoundFileName",lua_L2D_Model_getSoundFileName);
        tolua_function(tolua_S,"getWind",lua_L2D_Model_getWind);
        tolua_function(tolua_S,"multiplyParameter",lua_L2D_Model_multiplyParameter);
        tolua_function(tolua_S,"resetDragging",lua_L2D_Model_resetDragging);
        tolua_function(tolua_S,"setAcceleration",lua_L2D_Model_setAcceleration);
        tolua_function(tolua_S,"setAutoDragging",lua_L2D_Model_setAutoDragging);
        tolua_function(tolua_S,"setBlinkingInterval",lua_L2D_Model_setBlinkingInterval);
        tolua_function(tolua_S,"setBlinkingSettings",lua_L2D_Model_setBlinkingSettings);
        tolua_function(tolua_S,"setDebugRectEnable",lua_L2D_Model_setDebugRectEnable);
        tolua_function(tolua_S,"setDragging",lua_L2D_Model_setDragging);
        tolua_function(tolua_S,"setExpression",lua_L2D_Model_setExpression);
        tolua_function(tolua_S,"setGravity",lua_L2D_Model_setGravity);
        tolua_function(tolua_S,"setLipSync",lua_L2D_Model_setLipSync);
        tolua_function(tolua_S,"setLipValue",lua_L2D_Model_setLipValue);
        tolua_function(tolua_S,"setModelColor",lua_L2D_Model_setModelColor);
        tolua_function(tolua_S,"setModelOpacity",lua_L2D_Model_setModelOpacity);
        tolua_function(tolua_S,"setParameter",lua_L2D_Model_setParameter);
        tolua_function(tolua_S,"setPartOpacity",lua_L2D_Model_setPartOpacity);
        tolua_function(tolua_S,"setRandomExpression",lua_L2D_Model_setRandomExpression);
        tolua_function(tolua_S,"setSoundEventHandler",lua_L2D_Model_setSoundEventHandler);
        tolua_function(tolua_S,"setTouchDragging",lua_L2D_Model_setTouchDragging);
        tolua_function(tolua_S,"setWind",lua_L2D_Model_setWind);
        tolua_function(tolua_S,"startMotion",lua_L2D_Model_startMotion);
        tolua_function(tolua_S,"startRandomMotion",lua_L2D_Model_startRandomMotion);
        tolua_function(tolua_S,"create", lua_L2D_Model_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(l2d::Model).name();
    g_luaType[typeName] = "l2d.Model";
    g_typeCast["Model"] = "l2d.Model";
    return 1;
}
TOLUA_API int register_all_live2d(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, "l2d", 0);
	tolua_beginmodule(tolua_S, "l2d");

	lua_register_L2D_Model(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

