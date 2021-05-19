#include "lua_live2d_auto.hpp"
#include "L2DModel.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

#ifndef LUA_CHECK_COBJ_TYPE
	#ifdef LUA_DEBUG
		#define LUA_CHECK_COBJ_TYPE(L, TYPE, NAME) if(!tolua_isusertype((L), 1, (TYPE), 0, nullptr)) { return luaL_error((L), "invalid 'cobj' in '%s': '%s', expects '%s'", NAME, tolua_typename((L), 1), (TYPE)); }
	#else
		#define LUA_CHECK_COBJ_TYPE(L, TYPE, NAME) (void)(TYPE);
	#endif
#endif
#ifndef LUA_CHECK_COBJ
	#ifdef LUA_DEBUG
		#define LUA_CHECK_COBJ(L, COBJ, NAME) if(!(COBJ)) { return luaL_error((L), "invalid 'cobj' in '%s'", NAME); }
	#else
		#define LUA_CHECK_COBJ(L, COBJ, NAME)
	#endif
#endif
#ifndef LUA_CHECK_PARAMETER
	#define LUA_CHECK_PARAMETER(L, OK, NAME) if(!(OK)) { return luaL_error((L), "invalid arguments in '%s'", NAME); }
#endif
#ifndef LUA_PARAMETER_ERROR
	#define LUA_PARAMETER_ERROR(L, NAME, ARGC, EXPECT) return luaL_error((L), "wrong number of arguments in '%s': %d, expects %s", NAME, (ARGC), EXPECT);
#endif

int lua_L2D_Model_addParameter(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:addParameter";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		double arg1;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->addParameter(arg0, arg1);
		lua_settop(tolua_S, 1);
		return 1;
	}
	if (argc == 3) {
		std::string arg0;
		double arg1;
		double arg2;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 4, &arg2, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->addParameter(arg0, arg1, arg2);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2 to 3");
}
int lua_L2D_Model_areaHitTest(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:areaHitTest";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 3) {
		const char* arg0;
		double arg1;
		double arg2;
		ok &= lua_isstring(tolua_S, 2); if (ok) arg0 = luaL_checkstring(tolua_S, 2);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 4, &arg2, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->areaHitTest(arg0, arg1, arg2);
		tolua_pushboolean(tolua_S, (bool)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "3");
}
int lua_L2D_Model_create(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:create";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		std::string arg1;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_std_string(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = l2d::Model::create(arg0, arg1);
		object_to_luaval<l2d::Model>(tolua_S, "l2d.Model", (l2d::Model*)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2");
}
int lua_L2D_Model_getCanvasOrigin(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getCanvasOrigin";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getCanvasOrigin();
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getCanvasSize(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getCanvasSize";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getCanvasSize();
		size_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getDebugRectRenderer(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getDebugRectRenderer";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getDebugRectRenderer();
		object_to_luaval<cocos2d::DrawNode>(tolua_S, "cc.DrawNode", (cocos2d::DrawNode*)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getDirectory(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getDirectory";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getDirectory();
		lua_pushlstring(tolua_S, ret.c_str(), ret.length());
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getDrawableBlendMode(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getDrawableBlendMode";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getDrawableBlendMode(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getDrawableCount(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getDrawableCount";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getDrawableCount();
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getDrawableCulling(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getDrawableCulling";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getDrawableCulling(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getDrawableNames(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getDrawableNames";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getDrawableNames();
		ccvector_std_string_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getDrawableOpacity(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getDrawableOpacity";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getDrawableOpacity(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getExpressionNames(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getExpressionNames";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getExpressionNames();
		ccvector_std_string_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getGravity(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getGravity";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getGravity();
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getLipSync(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getLipSync";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getLipSync();
		tolua_pushboolean(tolua_S, (bool)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getLipValue(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getLipValue";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getLipValue();
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getMocVersion(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getMocVersion";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getMocVersion();
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getModelColor(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getModelColor";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getModelColor();
		color4b_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getModelOpacity(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getModelOpacity";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getModelOpacity();
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getMotionCount(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getMotionCount";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getMotionCount(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getMotionGroupNames(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getMotionGroupNames";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getMotionGroupNames();
		ccvector_std_string_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getParameter(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getParameter";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getParameter(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getParameterCount(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getParameterCount";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getParameterCount();
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getParameterDefault(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getParameterDefault";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getParameterDefault(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getParameterMax(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getParameterMax";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getParameterMax(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getParameterMin(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getParameterMin";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getParameterMin(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getParameterNames(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getParameterNames";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getParameterNames();
		ccvector_std_string_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getPartCount(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getPartCount";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getPartCount();
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getPartNames(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getPartNames";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getPartNames();
		ccvector_std_string_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getPartOpacity(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getPartOpacity";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getPartOpacity(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_getPartParents(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getPartParents";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getPartParents();
		ccvector_int_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getPixelsPerUnit(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getPixelsPerUnit";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getPixelsPerUnit();
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_getSoundFileName(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getSoundFileName";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		int arg1;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_int32(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getSoundFileName(arg0, arg1);
		lua_pushlstring(tolua_S, ret.c_str(), ret.length());
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2");
}
int lua_L2D_Model_getWind(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:getWind";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->getWind();
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_multiplyParameter(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:multiplyParameter";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		double arg1;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->multiplyParameter(arg0, arg1);
		lua_settop(tolua_S, 1);
		return 1;
	}
	if (argc == 3) {
		std::string arg0;
		double arg1;
		double arg2;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 4, &arg2, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->multiplyParameter(arg0, arg1, arg2);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2 to 3");
}
int lua_L2D_Model_resetDragging(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:resetDragging";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->resetDragging();
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_setAcceleration(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setAcceleration";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 3) {
		double arg0;
		double arg1;
		double arg2;
		ok &= luaval_to_number(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 4, &arg2, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setAcceleration(arg0, arg1, arg2);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "3");
}
int lua_L2D_Model_setAutoDragging(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setAutoDragging";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setAutoDragging(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setBlinkingInterval(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setBlinkingInterval";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setBlinkingInterval(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setBlinkingSettings(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setBlinkingSettings";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 3) {
		double arg0;
		double arg1;
		double arg2;
		ok &= luaval_to_number(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 4, &arg2, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setBlinkingSettings(arg0, arg1, arg2);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "3");
}
int lua_L2D_Model_setDebugRectEnable(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setDebugRectEnable";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setDebugRectEnable(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setDragging(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setDragging";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setDragging(arg0, arg1);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2");
}
int lua_L2D_Model_setExpression(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setExpression";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		const char* arg0;
		ok &= lua_isstring(tolua_S, 2); if (ok) arg0 = luaL_checkstring(tolua_S, 2);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->setExpression(arg0);
		tolua_pushboolean(tolua_S, (bool)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setGravity(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setGravity";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		cocos2d::Vec2 arg0;
		ok &= luaval_to_vec2(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setGravity(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setLipSync(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setLipSync";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setLipSync(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setLipValue(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setLipValue";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setLipValue(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setModelColor(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setModelColor";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		cocos2d::Color4B arg0;
		ok &=luaval_to_color4b(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setModelColor(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setModelOpacity(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setModelOpacity";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		uint16_t arg0;
		ok &= luaval_to_uint16(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setModelOpacity(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setParameter(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setParameter";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		double arg1;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setParameter(arg0, arg1);
		lua_settop(tolua_S, 1);
		return 1;
	}
	if (argc == 3) {
		std::string arg0;
		double arg1;
		double arg2;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 4, &arg2, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setParameter(arg0, arg1, arg2);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2 to 3");
}
int lua_L2D_Model_setPartOpacity(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setPartOpacity";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		double arg1;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setPartOpacity(arg0, arg1);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2");
}
int lua_L2D_Model_setRandomExpression(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setRandomExpression";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->setRandomExpression();
		tolua_pushboolean(tolua_S, (bool)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "0");
}
int lua_L2D_Model_setSoundEventHandler(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setSoundEventHandler";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
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
					return ;
				}
				lua_pushlstring(tolua_S, param0.c_str(), param0.length());
				tolua_pushnumber(tolua_S, (lua_Number)param1);
				lua_call(tolua_S, 2, 0);
			};
		};
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setSoundEventHandler(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_setTouchDragging(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setTouchDragging";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2, &arg0, LUA_FNAME);
		ok &= luaval_to_number(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setTouchDragging(arg0, arg1);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2");
}
int lua_L2D_Model_setWind(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:setWind";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		cocos2d::Vec2 arg0;
		ok &= luaval_to_vec2(tolua_S, 2, &arg0, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		cobj->setWind(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "1");
}
int lua_L2D_Model_startMotion(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:startMotion";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 3) {
		const char* arg0;
		int arg1;
		int arg2;
		ok &= lua_isstring(tolua_S, 2); if (ok) arg0 = luaL_checkstring(tolua_S, 2);
		ok &= luaval_to_int32(tolua_S, 3, &arg1, LUA_FNAME);
		ok &= luaval_to_int32(tolua_S, 4, &arg2, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->startMotion(arg0, arg1, arg2);
		tolua_pushboolean(tolua_S, (bool)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "3");
}
int lua_L2D_Model_startRandomMotion(lua_State* tolua_S)
{
	bool ok = true;
	constexpr auto LUA_OBJ_TYPE = "l2d.Model";
	constexpr auto LUA_FNAME = "l2d.Model:startRandomMotion";
	LUA_CHECK_COBJ_TYPE(tolua_S, LUA_OBJ_TYPE, LUA_FNAME);
	auto cobj = (l2d::Model*)tolua_tousertype(tolua_S, 1, nullptr);
	LUA_CHECK_COBJ(tolua_S, cobj, LUA_FNAME);
	const int argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		const char* arg0;
		int arg1;
		ok &= lua_isstring(tolua_S, 2); if (ok) arg0 = luaL_checkstring(tolua_S, 2);
		ok &= luaval_to_int32(tolua_S, 3, &arg1, LUA_FNAME);
		LUA_CHECK_PARAMETER(tolua_S, ok, LUA_FNAME);
		auto ret = cobj->startRandomMotion(arg0, arg1);
		tolua_pushboolean(tolua_S, (bool)ret);
		return 1;
	}
	LUA_PARAMETER_ERROR(tolua_S, LUA_FNAME, argc, "2");
}
static int lua_L2D_Model_finalize(lua_State* tolua_S)
{
	return 0;
}

int lua_register_L2D_Model(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "l2d.Model");
	tolua_cclass(tolua_S, "Model", "l2d.Model", "ccui.Widget", nullptr);

	tolua_beginmodule(tolua_S, "Model");
		tolua_function(tolua_S, "addParameter", lua_L2D_Model_addParameter);
		tolua_function(tolua_S, "areaHitTest", lua_L2D_Model_areaHitTest);
		tolua_function(tolua_S, "getCanvasOrigin", lua_L2D_Model_getCanvasOrigin);
		tolua_function(tolua_S, "getCanvasSize", lua_L2D_Model_getCanvasSize);
		tolua_function(tolua_S, "getDebugRectRenderer", lua_L2D_Model_getDebugRectRenderer);
		tolua_function(tolua_S, "getDirectory", lua_L2D_Model_getDirectory);
		tolua_function(tolua_S, "getDrawableBlendMode", lua_L2D_Model_getDrawableBlendMode);
		tolua_function(tolua_S, "getDrawableCount", lua_L2D_Model_getDrawableCount);
		tolua_function(tolua_S, "getDrawableCulling", lua_L2D_Model_getDrawableCulling);
		tolua_function(tolua_S, "getDrawableNames", lua_L2D_Model_getDrawableNames);
		tolua_function(tolua_S, "getDrawableOpacity", lua_L2D_Model_getDrawableOpacity);
		tolua_function(tolua_S, "getExpressionNames", lua_L2D_Model_getExpressionNames);
		tolua_function(tolua_S, "getGravity", lua_L2D_Model_getGravity);
		tolua_function(tolua_S, "getLipSync", lua_L2D_Model_getLipSync);
		tolua_function(tolua_S, "getLipValue", lua_L2D_Model_getLipValue);
		tolua_function(tolua_S, "getMocVersion", lua_L2D_Model_getMocVersion);
		tolua_function(tolua_S, "getModelColor", lua_L2D_Model_getModelColor);
		tolua_function(tolua_S, "getModelOpacity", lua_L2D_Model_getModelOpacity);
		tolua_function(tolua_S, "getMotionCount", lua_L2D_Model_getMotionCount);
		tolua_function(tolua_S, "getMotionGroupNames", lua_L2D_Model_getMotionGroupNames);
		tolua_function(tolua_S, "getParameter", lua_L2D_Model_getParameter);
		tolua_function(tolua_S, "getParameterCount", lua_L2D_Model_getParameterCount);
		tolua_function(tolua_S, "getParameterDefault", lua_L2D_Model_getParameterDefault);
		tolua_function(tolua_S, "getParameterMax", lua_L2D_Model_getParameterMax);
		tolua_function(tolua_S, "getParameterMin", lua_L2D_Model_getParameterMin);
		tolua_function(tolua_S, "getParameterNames", lua_L2D_Model_getParameterNames);
		tolua_function(tolua_S, "getPartCount", lua_L2D_Model_getPartCount);
		tolua_function(tolua_S, "getPartNames", lua_L2D_Model_getPartNames);
		tolua_function(tolua_S, "getPartOpacity", lua_L2D_Model_getPartOpacity);
		tolua_function(tolua_S, "getPartParents", lua_L2D_Model_getPartParents);
		tolua_function(tolua_S, "getPixelsPerUnit", lua_L2D_Model_getPixelsPerUnit);
		tolua_function(tolua_S, "getSoundFileName", lua_L2D_Model_getSoundFileName);
		tolua_function(tolua_S, "getWind", lua_L2D_Model_getWind);
		tolua_function(tolua_S, "multiplyParameter", lua_L2D_Model_multiplyParameter);
		tolua_function(tolua_S, "resetDragging", lua_L2D_Model_resetDragging);
		tolua_function(tolua_S, "setAcceleration", lua_L2D_Model_setAcceleration);
		tolua_function(tolua_S, "setAutoDragging", lua_L2D_Model_setAutoDragging);
		tolua_function(tolua_S, "setBlinkingInterval", lua_L2D_Model_setBlinkingInterval);
		tolua_function(tolua_S, "setBlinkingSettings", lua_L2D_Model_setBlinkingSettings);
		tolua_function(tolua_S, "setDebugRectEnable", lua_L2D_Model_setDebugRectEnable);
		tolua_function(tolua_S, "setDragging", lua_L2D_Model_setDragging);
		tolua_function(tolua_S, "setExpression", lua_L2D_Model_setExpression);
		tolua_function(tolua_S, "setGravity", lua_L2D_Model_setGravity);
		tolua_function(tolua_S, "setLipSync", lua_L2D_Model_setLipSync);
		tolua_function(tolua_S, "setLipValue", lua_L2D_Model_setLipValue);
		tolua_function(tolua_S, "setModelColor", lua_L2D_Model_setModelColor);
		tolua_function(tolua_S, "setModelOpacity", lua_L2D_Model_setModelOpacity);
		tolua_function(tolua_S, "setParameter", lua_L2D_Model_setParameter);
		tolua_function(tolua_S, "setPartOpacity", lua_L2D_Model_setPartOpacity);
		tolua_function(tolua_S, "setRandomExpression", lua_L2D_Model_setRandomExpression);
		tolua_function(tolua_S, "setSoundEventHandler", lua_L2D_Model_setSoundEventHandler);
		tolua_function(tolua_S, "setTouchDragging", lua_L2D_Model_setTouchDragging);
		tolua_function(tolua_S, "setWind", lua_L2D_Model_setWind);
		tolua_function(tolua_S, "startMotion", lua_L2D_Model_startMotion);
		tolua_function(tolua_S, "startRandomMotion", lua_L2D_Model_startRandomMotion);
		tolua_function(tolua_S, "create", lua_L2D_Model_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(l2d::Model).name();
	g_luaType[typeName] = "l2d.Model";
	g_typeCast["Model"] = "l2d.Model";
	return 1;
}

int register_all_live2d(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, "l2d", 0);
	tolua_beginmodule(tolua_S, "l2d");

	lua_register_L2D_Model(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

