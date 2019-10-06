#include "lua_x_L2D_auto.hpp"
#include "Model.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "UtilLuaConversion.h"

int lua_x_L2D_Model_setGravity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setGravity'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Vec2 arg0;

        ok &= luaval_to_vec2(tolua_S, 2, &arg0, "l2d.Model:setGravity");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setGravity'", nullptr);
            return 0;
        }
        cobj->setGravity(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setGravity",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setGravity'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setAcceleration(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setAcceleration'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
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
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setAcceleration'", nullptr);
            return 0;
        }
        cobj->setAcceleration(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setAcceleration",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setAcceleration'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setOnDraggingCallback(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setOnDraggingCallback'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;
		arg0 = toluafix_ref_function(tolua_S, 2, 0);
        if(!arg0)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setOnDraggingCallback'", nullptr);
            return 0;
        }
        cobj->setOnDraggingCallback(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setOnDraggingCallback",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setOnDraggingCallback'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getParameterValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getParameterValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getParameterValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getParameterValue'", nullptr);
            return 0;
        }
        double ret = cobj->getParameterValue(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterValue",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getParameterValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getDrawableCount(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getDrawableCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getDrawableCount'", nullptr);
            return 0;
        }
        int ret = cobj->getDrawableCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableCount",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getDrawableCount'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getDrawableOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getDrawableOpacity'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getDrawableOpacity");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getDrawableOpacity'", nullptr);
            return 0;
        }
        double ret = cobj->getDrawableOpacity(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableOpacity",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getDrawableOpacity'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setModelOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setModelOpacity'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        uint16_t arg0;

        ok &= luaval_to_uint16(tolua_S, 2,&arg0, "l2d.Model:setModelOpacity");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setModelOpacity'", nullptr);
            return 0;
        }
        cobj->setModelOpacity(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setModelOpacity",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setModelOpacity'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getDrawableBlendMode(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getDrawableBlendMode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getDrawableBlendMode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getDrawableBlendMode'", nullptr);
            return 0;
        }
        int ret = cobj->getDrawableBlendMode(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableBlendMode",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getDrawableBlendMode'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_areaHitTest(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_areaHitTest'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
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
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_areaHitTest'", nullptr);
            return 0;
        }
        bool ret = cobj->areaHitTest(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:areaHitTest",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_areaHitTest'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setDebugRectEnable(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setDebugRectEnable'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "l2d.Model:setDebugRectEnable");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setDebugRectEnable'", nullptr);
            return 0;
        }
        cobj->setDebugRectEnable(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setDebugRectEnable",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setDebugRectEnable'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setPartOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setPartOpacity'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        double arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:setPartOpacity");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setPartOpacity");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setPartOpacity'", nullptr);
            return 0;
        }
        cobj->setPartOpacity(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setPartOpacity",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setPartOpacity'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setLipValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setLipValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setLipValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setLipValue'", nullptr);
            return 0;
        }
        cobj->setLipValue(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setLipValue",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setLipValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getWind(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getWind'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getWind'", nullptr);
            return 0;
        }
        cocos2d::Vec2 ret = cobj->getWind();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getWind",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getWind'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getModelOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getModelOpacity'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getModelOpacity'", nullptr);
            return 0;
        }
        uint16_t ret = cobj->getModelOpacity();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getModelOpacity",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getModelOpacity'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getDrawableNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getDrawableNames'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getDrawableNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getDrawableNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableNames",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getDrawableNames'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setOnHitCallback(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setOnHitCallback'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;
		arg0 = toluafix_ref_function(tolua_S, 2, 0);
        if(!arg0)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setOnHitCallback'", nullptr);
            return 0;
        }
        cobj->setOnHitCallback(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setOnHitCallback",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setOnHitCallback'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setWind(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setWind'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Vec2 arg0;

        ok &= luaval_to_vec2(tolua_S, 2, &arg0, "l2d.Model:setWind");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setWind'", nullptr);
            return 0;
        }
        cobj->setWind(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setWind",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setWind'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getMotionGroupNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getMotionGroupNames'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getMotionGroupNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getMotionGroupNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getMotionGroupNames",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getMotionGroupNames'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getParameterMinimumValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getParameterMinimumValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getParameterMinimumValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getParameterMinimumValue'", nullptr);
            return 0;
        }
        double ret = cobj->getParameterMinimumValue(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterMinimumValue",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getParameterMinimumValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getPartOpacity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getPartOpacity'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getPartOpacity");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getPartOpacity'", nullptr);
            return 0;
        }
        double ret = cobj->getPartOpacity(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getPartOpacity",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getPartOpacity'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_startMotion(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_startMotion'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
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
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_startMotion'", nullptr);
            return 0;
        }
        bool ret = cobj->startMotion(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:startMotion",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_startMotion'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_startRandomMotion(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_startRandomMotion'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        const char* arg0;
        int arg1;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "l2d.Model:startRandomMotion"); arg0 = arg0_tmp.c_str();

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "l2d.Model:startRandomMotion");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_startRandomMotion'", nullptr);
            return 0;
        }
        bool ret = cobj->startRandomMotion(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:startRandomMotion",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_startRandomMotion'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getPartCount(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getPartCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getPartCount'", nullptr);
            return 0;
        }
        int ret = cobj->getPartCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getPartCount",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getPartCount'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getCanvasRect(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getCanvasRect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getCanvasRect'", nullptr);
            return 0;
        }
        cocos2d::Rect ret = cobj->getCanvasRect();
        rect_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getCanvasRect",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getCanvasRect'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_addParameterValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_addParameterValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        double arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:addParameterValue");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:addParameterValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_addParameterValue'", nullptr);
            return 0;
        }
        cobj->addParameterValue(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:addParameterValue");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:addParameterValue");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:addParameterValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_addParameterValue'", nullptr);
            return 0;
        }
        cobj->addParameterValue(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:addParameterValue",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_addParameterValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getParameterDefaultValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getParameterDefaultValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getParameterDefaultValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getParameterDefaultValue'", nullptr);
            return 0;
        }
        double ret = cobj->getParameterDefaultValue(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterDefaultValue",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getParameterDefaultValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getParameterNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getParameterNames'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getParameterNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getParameterNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterNames",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getParameterNames'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setParameterValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setParameterValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        double arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:setParameterValue");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setParameterValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setParameterValue'", nullptr);
            return 0;
        }
        cobj->setParameterValue(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:setParameterValue");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setParameterValue");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:setParameterValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setParameterValue'", nullptr);
            return 0;
        }
        cobj->setParameterValue(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setParameterValue",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setParameterValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setBlinkingSettings(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setBlinkingSettings'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
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
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setBlinkingSettings'", nullptr);
            return 0;
        }
        cobj->setBlinkingSettings(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setBlinkingSettings",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setBlinkingSettings'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getPartNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getPartNames'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getPartNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getPartNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getPartNames",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getPartNames'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setRandomExpression(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setRandomExpression'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setRandomExpression'", nullptr);
            return 0;
        }
        bool ret = cobj->setRandomExpression();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setRandomExpression",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setRandomExpression'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getMotionNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getMotionNames'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getMotionNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getMotionNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getMotionNames",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getMotionNames'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getGravity(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getGravity'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getGravity'", nullptr);
            return 0;
        }
        cocos2d::Vec2 ret = cobj->getGravity();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getGravity",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getGravity'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getDebugRectRenderer(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getDebugRectRenderer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getDebugRectRenderer'", nullptr);
            return 0;
        }
        cocos2d::DrawNode* ret = cobj->getDebugRectRenderer();
        lstg::lua::native_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDebugRectRenderer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getDebugRectRenderer'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getParameterCount(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getParameterCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getParameterCount'", nullptr);
            return 0;
        }
        int ret = cobj->getParameterCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterCount",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getParameterCount'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setExpression(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setExpression'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        const char* arg0;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "l2d.Model:setExpression"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setExpression'", nullptr);
            return 0;
        }
        bool ret = cobj->setExpression(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setExpression",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setExpression'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setModelColor(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setModelColor'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Color4B arg0;

        ok &=luaval_to_color4b(tolua_S, 2, &arg0, "l2d.Model:setModelColor");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setModelColor'", nullptr);
            return 0;
        }
        cobj->setModelColor(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setModelColor",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setModelColor'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setBlinkingInterval(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setBlinkingInterval'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setBlinkingInterval");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setBlinkingInterval'", nullptr);
            return 0;
        }
        cobj->setBlinkingInterval(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setBlinkingInterval",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setBlinkingInterval'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getLipValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getLipValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getLipValue'", nullptr);
            return 0;
        }
        double ret = cobj->getLipValue();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getLipValue",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getLipValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getParameterMaximumValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getParameterMaximumValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getParameterMaximumValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getParameterMaximumValue'", nullptr);
            return 0;
        }
        double ret = cobj->getParameterMaximumValue(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getParameterMaximumValue",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getParameterMaximumValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getDrawableCulling(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getDrawableCulling'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:getDrawableCulling");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getDrawableCulling'", nullptr);
            return 0;
        }
        int ret = cobj->getDrawableCulling(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getDrawableCulling",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getDrawableCulling'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getModelColor(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getModelColor'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getModelColor'", nullptr);
            return 0;
        }
        cocos2d::Color4B ret = cobj->getModelColor();
        color4b_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getModelColor",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getModelColor'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_setDragging(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_setDragging'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "l2d.Model:setDragging");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:setDragging");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_setDragging'", nullptr);
            return 0;
        }
        cobj->setDragging(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:setDragging",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_setDragging'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_multiplyParameterValue(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_multiplyParameterValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        double arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:multiplyParameterValue");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:multiplyParameterValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_multiplyParameterValue'", nullptr);
            return 0;
        }
        cobj->multiplyParameterValue(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        double arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "l2d.Model:multiplyParameterValue");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "l2d.Model:multiplyParameterValue");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "l2d.Model:multiplyParameterValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_multiplyParameterValue'", nullptr);
            return 0;
        }
        cobj->multiplyParameterValue(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:multiplyParameterValue",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_multiplyParameterValue'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getExpressionNames(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getExpressionNames'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getExpressionNames'", nullptr);
            return 0;
        }
        std::vector<std::string> ret = cobj->getExpressionNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getExpressionNames",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getExpressionNames'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_getCanvasSize(lua_State* tolua_S)
{
    int argc = 0;
    l2d::Model* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"l2d.Model",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (l2d::Model*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_x_L2D_Model_getCanvasSize'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_getCanvasSize'", nullptr);
            return 0;
        }
        cocos2d::Size ret = cobj->getCanvasSize();
        size_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "l2d.Model:getCanvasSize",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_getCanvasSize'.",&tolua_err);
#endif

    return 0;
}
int lua_x_L2D_Model_create(lua_State* tolua_S)
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
            tolua_error(tolua_S,"invalid arguments in function 'lua_x_L2D_Model_create'", nullptr);
            return 0;
        }
        l2d::Model* ret = l2d::Model::create(arg0, arg1);
        lstg::lua::native_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "l2d.Model:create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_x_L2D_Model_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_x_L2D_Model_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Model)");
    return 0;
}

int lua_register_x_L2D_Model(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"l2d.Model");
    tolua_cclass(tolua_S,"Model","l2d.Model","ccui.Widget",nullptr);

    tolua_beginmodule(tolua_S,"Model");
        tolua_function(tolua_S,"setGravity",lua_x_L2D_Model_setGravity);
        tolua_function(tolua_S,"setAcceleration",lua_x_L2D_Model_setAcceleration);
        tolua_function(tolua_S,"setOnDraggingCallback",lua_x_L2D_Model_setOnDraggingCallback);
        tolua_function(tolua_S,"getParameterValue",lua_x_L2D_Model_getParameterValue);
        tolua_function(tolua_S,"getDrawableCount",lua_x_L2D_Model_getDrawableCount);
        tolua_function(tolua_S,"getDrawableOpacity",lua_x_L2D_Model_getDrawableOpacity);
        tolua_function(tolua_S,"setModelOpacity",lua_x_L2D_Model_setModelOpacity);
        tolua_function(tolua_S,"getDrawableBlendMode",lua_x_L2D_Model_getDrawableBlendMode);
        tolua_function(tolua_S,"areaHitTest",lua_x_L2D_Model_areaHitTest);
        tolua_function(tolua_S,"setDebugRectEnable",lua_x_L2D_Model_setDebugRectEnable);
        tolua_function(tolua_S,"setPartOpacity",lua_x_L2D_Model_setPartOpacity);
        tolua_function(tolua_S,"setLipValue",lua_x_L2D_Model_setLipValue);
        tolua_function(tolua_S,"getWind",lua_x_L2D_Model_getWind);
        tolua_function(tolua_S,"getModelOpacity",lua_x_L2D_Model_getModelOpacity);
        tolua_function(tolua_S,"getDrawableNames",lua_x_L2D_Model_getDrawableNames);
        tolua_function(tolua_S,"setOnHitCallback",lua_x_L2D_Model_setOnHitCallback);
        tolua_function(tolua_S,"setWind",lua_x_L2D_Model_setWind);
        tolua_function(tolua_S,"getMotionGroupNames",lua_x_L2D_Model_getMotionGroupNames);
        tolua_function(tolua_S,"getParameterMinimumValue",lua_x_L2D_Model_getParameterMinimumValue);
        tolua_function(tolua_S,"getPartOpacity",lua_x_L2D_Model_getPartOpacity);
        tolua_function(tolua_S,"startMotion",lua_x_L2D_Model_startMotion);
        tolua_function(tolua_S,"startRandomMotion",lua_x_L2D_Model_startRandomMotion);
        tolua_function(tolua_S,"getPartCount",lua_x_L2D_Model_getPartCount);
        tolua_function(tolua_S,"getCanvasRect",lua_x_L2D_Model_getCanvasRect);
        tolua_function(tolua_S,"addParameterValue",lua_x_L2D_Model_addParameterValue);
        tolua_function(tolua_S,"getParameterDefaultValue",lua_x_L2D_Model_getParameterDefaultValue);
        tolua_function(tolua_S,"getParameterNames",lua_x_L2D_Model_getParameterNames);
        tolua_function(tolua_S,"setParameterValue",lua_x_L2D_Model_setParameterValue);
        tolua_function(tolua_S,"setBlinkingSettings",lua_x_L2D_Model_setBlinkingSettings);
        tolua_function(tolua_S,"getPartNames",lua_x_L2D_Model_getPartNames);
        tolua_function(tolua_S,"setRandomExpression",lua_x_L2D_Model_setRandomExpression);
        tolua_function(tolua_S,"getMotionNames",lua_x_L2D_Model_getMotionNames);
        tolua_function(tolua_S,"getGravity",lua_x_L2D_Model_getGravity);
        tolua_function(tolua_S,"getDebugRectRenderer",lua_x_L2D_Model_getDebugRectRenderer);
        tolua_function(tolua_S,"getParameterCount",lua_x_L2D_Model_getParameterCount);
        tolua_function(tolua_S,"setExpression",lua_x_L2D_Model_setExpression);
        tolua_function(tolua_S,"setModelColor",lua_x_L2D_Model_setModelColor);
        tolua_function(tolua_S,"setBlinkingInterval",lua_x_L2D_Model_setBlinkingInterval);
        tolua_function(tolua_S,"getLipValue",lua_x_L2D_Model_getLipValue);
        tolua_function(tolua_S,"getParameterMaximumValue",lua_x_L2D_Model_getParameterMaximumValue);
        tolua_function(tolua_S,"getDrawableCulling",lua_x_L2D_Model_getDrawableCulling);
        tolua_function(tolua_S,"getModelColor",lua_x_L2D_Model_getModelColor);
        tolua_function(tolua_S,"setDragging",lua_x_L2D_Model_setDragging);
        tolua_function(tolua_S,"multiplyParameterValue",lua_x_L2D_Model_multiplyParameterValue);
        tolua_function(tolua_S,"getExpressionNames",lua_x_L2D_Model_getExpressionNames);
        tolua_function(tolua_S,"getCanvasSize",lua_x_L2D_Model_getCanvasSize);
        tolua_function(tolua_S,"create", lua_x_L2D_Model_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(l2d::Model).name();
    g_luaType[typeName] = "l2d.Model";
    g_typeCast["Model"] = "l2d.Model";
    return 1;
}
TOLUA_API int register_all_x_L2D(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"l2d",0);
	tolua_beginmodule(tolua_S,"l2d");

	lua_register_x_L2D_Model(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

