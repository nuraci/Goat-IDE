local enums = {}
local structs = {}

------------------------------------------------------------------------------

table.insert(enums, {
	cname = 'class_code',
	values = {
		CLASS_PER_INTERFACE = 'per_interface',
		CLASS_AUDIO = 'audio',
		CLASS_COMM = 'comm',
		CLASS_HID = 'hid',
		CLASS_PRINTER = 'printer',
		CLASS_PTP = 'ptp',
		CLASS_MASS_STORAGE = 'mass_storage',
		CLASS_HUB = 'hub',
		CLASS_DATA = 'data',
		CLASS_WIRELESS = 'wireless',
		CLASS_APPLICATION = 'application',
		CLASS_VENDOR_SPEC = 'vendor_spec',
	},
})

table.insert(enums, {
	cname = 'descriptor_type',
	values = {
		DT_DEVICE = 'device',
		DT_CONFIG = 'config',
		DT_STRING = 'string',
		DT_INTERFACE = 'interface',
		DT_ENDPOINT = 'endpoint',
		DT_HID = 'hid',
		DT_REPORT = 'report',
		DT_PHYSICAL = 'physical',
		DT_HUB = 'hub',
	},
})

table.insert(enums, {
	cname = 'endpoint_direction',
	values = {
		ENDPOINT_IN = 'in',
		ENDPOINT_OUT = 'out',
	},
})

table.insert(enums, {
	cname = 'transfer_type',
	values = {
		TRANSFER_TYPE_CONTROL = 'control',
		TRANSFER_TYPE_ISOCHRONOUS = 'isochronous',
		TRANSFER_TYPE_BULK = 'bulk',
		TRANSFER_TYPE_INTERRUPT = 'interrupt',
	},
})

table.insert(enums, {
	cname = 'iso_sync_type',
	values = {
		ISO_SYNC_TYPE_NONE = 'none',
		ISO_SYNC_TYPE_ASYNC = 'async',
		ISO_SYNC_TYPE_ADAPTIVE = 'adaptive',
		ISO_SYNC_TYPE_SYNC = 'sync',
	},
})

table.insert(enums, {
	cname = 'iso_usage_type',
	values = {
		ISO_USAGE_TYPE_DATA = 'data',
		ISO_USAGE_TYPE_FEEDBACK = 'feedback',
		ISO_USAGE_TYPE_IMPLICIT = 'implicit',
	},
})

table.insert(enums, {
	cname = 'standard_request',
	values = {
		REQUEST_GET_STATUS = 'get_status',
		REQUEST_CLEAR_FEATURE = 'clear_feature',
		REQUEST_SET_FEATURE = 'set_feature',
		REQUEST_SET_ADDRESS = 'set_address',
		REQUEST_GET_DESCRIPTOR = 'get_descriptor',
		REQUEST_SET_DESCRIPTOR = 'set_descriptor',
		REQUEST_GET_CONFIGURATION = 'get_configuration',
		REQUEST_SET_CONFIGURATION = 'set_configuration',
		REQUEST_GET_INTERFACE = 'get_interface',
		REQUEST_SET_INTERFACE = 'set_interface',
		REQUEST_SYNCH_FRAME = 'synch_frame',
	},
})

table.insert(enums, {
	cname = 'request_type',
	values = {
		REQUEST_TYPE_STANDARD = 'standard',
		REQUEST_TYPE_CLASS = 'class',
		REQUEST_TYPE_VENDOR = 'vendor',
		REQUEST_TYPE_RESERVED = 'reserved',
	},
})

table.insert(enums, {
	cname = 'request_recipient',
	values = {
		RECIPIENT_DEVICE = 'device',
		RECIPIENT_INTERFACE = 'interface',
		RECIPIENT_ENDPOINT = 'endpoint',
		RECIPIENT_OTHER = 'other',
	},
})

table.insert(enums, {
	cname = 'error',
	values = {
--		SUCCESS = 'success',
		ERROR_IO = 'io',
		ERROR_INVALID_PARAM = 'invalid_param',
		ERROR_ACCESS = 'access',
		ERROR_NO_DEVICE = 'no_device',
		ERROR_NOT_FOUND = 'not_found',
		ERROR_BUSY = 'busy',
		ERROR_TIMEOUT = 'timeout',
		ERROR_OVERFLOW = 'overflow',
		ERROR_PIPE = 'pipe',
		ERROR_INTERRUPTED = 'interrupted',
		ERROR_NO_MEM = 'no_mem',
		ERROR_NOT_SUPPORTED = 'not_supported',
		ERROR_OTHER = 'other',
	},
})

------------------------------------------------------------------------------

table.insert(structs, {
	cname = 'device_descriptor',
	fields = {
		{name='bLength', ctype='uint8_t', luatype='number'},
		{name='bDescriptorType', ctype='uint8_t', luatype='number'},
		{name='bcdUSB', ctype='uint16_t', luatype='number'},
		{name='bDeviceClass', ctype='uint8_t', luatype='number'},
		{name='bDeviceSubClass', ctype='uint8_t', luatype='number'},
		{name='bDeviceProtocol', ctype='uint8_t', luatype='number'},
		{name='bMaxPacketSize0', ctype='uint8_t', luatype='number'},
		{name='idVendor', ctype='uint16_t', luatype='number'},
		{name='idProduct', ctype='uint16_t', luatype='number'},
		{name='bcdDevice', ctype='uint16_t', luatype='number'},
		{name='iManufacturer', ctype='uint8_t', luatype='number'},
		{name='iProduct', ctype='uint8_t', luatype='number'},
		{name='iSerialNumber', ctype='uint8_t', luatype='number'},
		{name='bNumConfigurations', ctype='uint8_t', luatype='number'},
	},
})

table.insert(structs, {
	cname = 'endpoint_descriptor',
	fields = {
		{name='bLength', ctype='uint8_t', luatype='number'},
		{name='bDescriptorType', ctype='uint8_t', luatype='number'},
		{name='bEndpointAddress', ctype='uint8_t', luatype='number'},
		{name='bmAttributes', ctype='uint8_t', luatype='number'},
		{name='wMaxPacketSize', ctype='uint16_t', luatype='number'},
		{name='bInterval', ctype='uint8_t', luatype='number'},
		{name='bRefresh', ctype='uint8_t', luatype='number'},
		{name='bSynchAddress', ctype='uint8_t', luatype='number'},
		{name='extra', ctype='const unsigned char*', luatype='string', size='extra_length', size_ctype='int'},
	},
})

table.insert(structs, {
	cname = 'interface_descriptor',
	fields = {
		{name='bLength', ctype='uint8_t', luatype='number'},
		{name='bDescriptorType', ctype='uint8_t', luatype='number'},
		{name='bInterfaceNumber', ctype='uint8_t', luatype='number'},
		{name='bAlternateSetting', ctype='uint8_t', luatype='number'},
		{name='bNumEndpoints', ctype='uint8_t', luatype='number'},
		{name='bInterfaceClass', ctype='uint8_t', luatype='number'},
		{name='bInterfaceSubClass', ctype='uint8_t', luatype='number'},
		{name='bInterfaceProtocol', ctype='uint8_t', luatype='number'},
		{name='iInterface', ctype='uint8_t', luatype='number'},
		{name='endpoint', ctype='struct libusb_endpoint_descriptor*'},
		{name='extra', ctype='const unsigned char*', luatype='string', size='extra_length', size_ctype='int'},
	},
})

table.insert(structs, {
	cname = 'interface',
	fields = {
		{name='altsetting', ctype='struct libusb_interface_descriptor*'},
		{name='num_altsetting', ctype='int', luatype='number'},
	},
})

table.insert(structs, {
	cname = 'config_descriptor',
	fields = {
		{name='bLength', ctype='uint8_t', luatype='number'},
		{name='bDescriptorType', ctype='uint8_t', luatype='number'},
		{name='wTotalLength', ctype='uint16_t', luatype='number'},
		{name='bNumInterfaces', ctype='uint8_t', luatype='number'},
		{name='bConfigurationValue', ctype='uint8_t', luatype='number'},
		{name='iConfiguration', ctype='uint8_t', luatype='number'},
		{name='bmAttributes', ctype='uint8_t', luatype='number'},
		{name='MaxPower', ctype='uint8_t', luatype='number'},
		{name='interface', ctype='struct libusb_interface*'},
		{name='extra', ctype='const unsigned char*', luatype='string', size='extra_length', size_ctype='int'},
	},
	gc = true,
})

table.insert(structs, {
	cname = 'device',
	gc = true,
})

table.insert(structs, {
	cname = 'device_handle',
	gc = true,
})

------------------------------------------------------------------------------

local enums_h = assert(io.open('enums.h', 'wb'))
local enums_c = assert(io.open('enums.c', 'wb'))
local structs_h = assert(io.open('structs.h', 'wb'))
local structs_c = assert(io.open('structs.c', 'wb'))

enums_h:write([[
/* this is a generated file, see gen.lua */
#include <lua.h>
#ifdef WIN32
#include "include/libusb.h"
#else
#include <libusb-1.0/libusb.h>
#endif

]])

enums_c:write([[
/* this is a generated file, see gen.lua */
#include "enums.h"

#include <lua.h>
#include <lauxlib.h>
#ifdef WIN32
#include "include/libusb.h"
#else
#include <libusb-1.0/libusb.h>
#endif
#include "compat.h"

]])

for _,enum in ipairs(enums) do
	local cname = enum.cname
	enums_h:write([[
enum libusb_]]..cname..[[ luausb_to_]]..cname..[[(lua_State* L, int index);
]])
	enums_c:write([[
enum libusb_]]..cname..[[ luausb_to_]]..cname..[[(lua_State* L, int index)
{
	switch (lua_type(L, index))
	{
	case LUA_TNUMBER:
		return lua_tonumber(L, index);
	case LUA_TSTRING:
		{
			enum libusb_]]..cname..[[ result;
			lua_getfield(L, lua_upvalueindex(1), "]]..cname..[[");
			lua_pushvalue(L, index);
			lua_gettable(L, -2);
			if (lua_type(L, -1)==LUA_TNUMBER)
				result = lua_tonumber(L, -1);
			else
				result = 0;
			lua_pop(L, 2);
			return result;
		}
	default:
		return 0;
	}
}

]])

	enums_h:write([[
int luausb_is_]]..cname..[[(lua_State* L, int index);
]])
	enums_c:write([[
int luausb_is_]]..cname..[[(lua_State* L, int index)
{
	switch (lua_type(L, index))
	{
	case LUA_TNUMBER:
		return 1;
	case LUA_TSTRING:
		{
			int result;
			lua_getfield(L, lua_upvalueindex(1), "]]..cname..[[");
			lua_pushvalue(L, index);
			lua_gettable(L, -2);
			if (lua_type(L, -1)==LUA_TNUMBER)
				result = 1;
			else
				result = 0;
			lua_pop(L, 2);
			return result;
		}
	default:
		return 0;
	}
}

]])

	enums_h:write([[
enum libusb_]]..cname..[[ luausb_check_]]..cname..[[(lua_State* L, int narg);
]])
	enums_c:write([[
enum libusb_]]..cname..[[ luausb_check_]]..cname..[[(lua_State* L, int narg)
{
	switch (lua_type(L, narg))
	{
	case LUA_TNUMBER:
		return lua_tonumber(L, narg);
	case LUA_TSTRING:
		{
			enum libusb_]]..cname..[[ result;
			lua_getfield(L, lua_upvalueindex(1), "]]..cname..[[");
			lua_pushvalue(L, narg);
			lua_gettable(L, -2);
			if (lua_type(L, -1)==LUA_TNUMBER)
				result = lua_tonumber(L, -1);
			else
			{
				typeerror(L, narg, "enum libusb_]]..cname..[[");
				return 0;
			}
			lua_pop(L, 2);
			return result;
		}
	default:
		typeerror(L, narg, "enum libusb_]]..cname..[[");
		return 0;
	}
}

]])

	enums_h:write([[
enum libusb_]]..cname..[[ luausb_opt_]]..cname..[[(lua_State* L, int narg, enum libusb_]]..cname..[[ d);
]])
	enums_c:write([[
enum libusb_]]..cname..[[ luausb_opt_]]..cname..[[(lua_State* L, int narg, enum libusb_]]..cname..[[ d)
{
	if (lua_isnoneornil(L, narg))
		return d;
	else
		return luausb_check_]]..cname..[[(L, narg);
}

]])
end

enums_h:write([[
void luausb_init_enums(lua_State* L);
]])

enums_c:write([[
void luausb_init_enums(lua_State* L)
{
	/* ..., env */
]])

for _,enum in ipairs(enums) do
	for cname in pairs(enum.values) do
		enums_c:write([[
	lua_pushnumber(L, LIBUSB_]]..cname..[[); lua_setfield(L, -2, "]]..cname..[[");
]])
	end
end

for _,enum in ipairs(enums) do
	local cname = enum.cname
	enums_c:write([[
	lua_newtable(L); /* ..., env, t */
]])
	for cname,luaname in pairs(enum.values) do
		enums_c:write([[
	lua_pushnumber(L, LIBUSB_]]..cname..[[); lua_setfield(L, -2, "LIBUSB_]]..cname..[[");
	lua_pushnumber(L, LIBUSB_]]..cname..[[); lua_setfield(L, -2, "]]..luaname..[[");
]])
	end
	enums_c:write([[
	lua_setfield(L, -2, "]]..cname..[["); /* ..., env */
	
]])
end

enums_c:write([[
}

]])

structs_h:write([[
/* this is a generated file, see gen.lua */
#include <lua.h>
#include <libusb-1.0/libusb.h>

]])

structs_c:write([[
/* this is a generated file, see gen.lua */
#include "structs.h"

#include <lauxlib.h>
#include "compat.h"

static int luausb_generic_index(lua_State* L)
{
	lua_getmetatable(L, 1);
	/* get a getter and call it */
	lua_getfield(L, -1, "getters");
	lua_pushvalue(L, 2);
	lua_gettable(L, -2);
	if (!lua_isnil(L, -1))
	{
		lua_pushvalue(L, 1);
		lua_call(L, 1, 1);
		return 1;
	}
	lua_pop(L, 2);
	/* get a method */
	lua_getfield(L, -1, "methods");
	lua_pushvalue(L, 2);
	lua_gettable(L, -2);
	return 1;
}

static int luausb_generic_tostring(lua_State* L)
{
	lua_getmetatable(L, 1);
	lua_getfield(L, -1, "typename");
	lua_pushfstring(L, "%s: %p", lua_tostring(L, -1), lua_touserdata(L, 1));
	return 1;
}

]])

for _,struct in ipairs(structs) do
	local cname = struct.cname
	structs_h:write([[
struct libusb_]]..cname..[[* luausb_to_]]..cname..[[(lua_State* L, int index);
]])
	structs_c:write([[
struct libusb_]]..cname..[[* luausb_to_]]..cname..[[(lua_State* L, int index)
{
	struct libusb_]]..cname..[[* udata;
	udata = *(struct libusb_]]..cname..[[**)lua_touserdata(L, index);
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

]])
	
	structs_h:write([[
struct libusb_]]..cname..[[* luausb_check_]]..cname..[[(lua_State* L, int index);
]])
	structs_c:write([[
struct libusb_]]..cname..[[* luausb_check_]]..cname..[[(lua_State* L, int index)
{
	struct libusb_]]..cname..[[* udata;
	udata = *(struct libusb_]]..cname..[[**)luaL_checkudata(L, index, "struct libusb_]]..cname..[[");
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

]])
	
	structs_h:write([[
void luausb_push_]]..cname..[[(lua_State* L, const struct libusb_]]..cname..[[* value, int owner);
]])
	structs_c:write([[
void luausb_push_]]..cname..[[(lua_State* L, const struct libusb_]]..cname..[[* value, int owner)
{
	if (owner < 0) owner = lua_gettop(L) + 1 + owner;
	const struct libusb_]]..cname..[[** udata;
	udata = (const struct libusb_]]..cname..[[**)lua_newuserdata(L, sizeof(struct libusb_]]..cname..[[*));
	*udata = value;
	luaL_getmetatable(L, "struct libusb_]]..cname..[[");
	lua_setmetatable(L, -2);
	if (owner != 0)
	{
		lua_createtable(L, 1, 0);
		lua_pushvalue(L, owner);
		lua_rawseti(L, -2, 1);
		setuservalue(L, -2);
	}
}

]])

	-- getters
	if struct.fields then
		for _,field in ipairs(struct.fields) do
			if field.luatype=='number' then
				structs_c:write([[
static int luausb_get_]]..cname..[[_]]..field.name..[[(lua_State* L)
{
	struct libusb_]]..cname..[[* udata;
	udata = luausb_to_]]..cname..[[(L, 1);
	lua_pushnumber(L, udata->]]..field.name..[[);
	return 1;
}

]])
			elseif field.luatype=='string' then
				structs_c:write([[
static int luausb_get_]]..cname..[[_]]..field.name..[[(lua_State* L)
{
	struct libusb_]]..cname..[[* udata;
	udata = luausb_to_]]..cname..[[(L, 1);
	if (udata->]]..field.name..[[)
	{
]])
				if type(field.size)=='string' then
					structs_c:write([[
		lua_pushlstring(L, (const char*)udata->]]..field.name..[[, udata->]]..field.size..[[);
]])
				elseif type(field.size)=='number' then
					structs_c:write([[
		lua_pushlstring(L, (const char*)udata->]]..field.name..[[, ]]..field.size..[[);
]])
				elseif type(field.size)=='nil' then
					structs_c:write([[
		lua_pushstring(L, (const char*)udata->]]..field.name..[[);
]])
				else
					error("unsupported field size")
				end
				structs_c:write([[
	}
	else
		lua_pushnil(L);
	return 1;
}

]])
			else
				structs_c:write([[
int luausb_get_]]..cname..[[_]]..field.name..[[(lua_State* L);

]])
			end
		end
	end
	structs_c:write([[
static struct luaL_Reg libusb_]]..cname..[[__getters[] = {
]])
	if struct.fields then
		for _,field in ipairs(struct.fields) do
			structs_c:write([[
	{"]]..field.name..[[", luausb_get_]]..cname..[[_]]..field.name..[[},
]])
		end
	end
	structs_c:write([[
	{0, 0},
};
]])
	
	-- metamethods
	if struct.gc then
		structs_c:write([[
int luausb_]]..cname..[[_gc(lua_State* L);
]])
	end
	structs_c:write([[
static struct luaL_Reg libusb_]]..cname..[[__metamethods[] = {
	{"__index", luausb_generic_index},
	{"__tostring", luausb_generic_tostring},
]])
	if struct.gc then
		structs_c:write([[
	{"__gc", luausb_]]..cname..[[_gc},
]])
	end
	structs_c:write([[
	{0, 0},
};
]])
	
	-- methods
	structs_c:write([[
extern struct luaL_Reg libusb_]]..cname..[[__methods[];

]])
end

structs_h:write([[
void luausb_init_structs(lua_State* L);
]])
structs_c:write([[
void luausb_init_structs(lua_State* L)
{
]])

for _,struct in ipairs(structs) do
	local cname = struct.cname
structs_c:write([[
	/* ]]..cname..[[ */
	luaL_newmetatable(L, "struct libusb_]]..cname..[[");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_]]..cname..[[__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_]]..cname..[[__methods, 1);
	lua_setfield(L, -2, "methods");
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_]]..cname..[[__getters, 1);
	lua_setfield(L, -2, "getters");
	lua_pushliteral(L, "]]..cname..[[");
	lua_setfield(L, -2, "typename");
	lua_pop(L, 1);
]])
end

structs_c:write([[
}
]])

