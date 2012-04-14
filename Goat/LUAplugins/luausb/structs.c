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

struct libusb_device_descriptor* luausb_to_device_descriptor(lua_State* L, int index)
{
	struct libusb_device_descriptor* udata;
	udata = *(struct libusb_device_descriptor**)lua_touserdata(L, index);
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

struct libusb_device_descriptor* luausb_check_device_descriptor(lua_State* L, int index)
{
	struct libusb_device_descriptor* udata;
	udata = *(struct libusb_device_descriptor**)luaL_checkudata(L, index, "struct libusb_device_descriptor");
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

void luausb_push_device_descriptor(lua_State* L, const struct libusb_device_descriptor* value, int owner)
{
	if (owner < 0) owner = lua_gettop(L) + 1 + owner;
	const struct libusb_device_descriptor** udata;
	udata = (const struct libusb_device_descriptor**)lua_newuserdata(L, sizeof(struct libusb_device_descriptor*));
	*udata = value;
	luaL_getmetatable(L, "struct libusb_device_descriptor");
	lua_setmetatable(L, -2);
	if (owner != 0)
	{
		lua_createtable(L, 1, 0);
		lua_pushvalue(L, owner);
		lua_rawseti(L, -2, 1);
		setuservalue(L, -2);
	}
}

static int luausb_get_device_descriptor_bLength(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bLength);
	return 1;
}

static int luausb_get_device_descriptor_bDescriptorType(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bDescriptorType);
	return 1;
}

static int luausb_get_device_descriptor_bcdUSB(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bcdUSB);
	return 1;
}

static int luausb_get_device_descriptor_bDeviceClass(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bDeviceClass);
	return 1;
}

static int luausb_get_device_descriptor_bDeviceSubClass(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bDeviceSubClass);
	return 1;
}

static int luausb_get_device_descriptor_bDeviceProtocol(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bDeviceProtocol);
	return 1;
}

static int luausb_get_device_descriptor_bMaxPacketSize0(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bMaxPacketSize0);
	return 1;
}

static int luausb_get_device_descriptor_idVendor(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->idVendor);
	return 1;
}

static int luausb_get_device_descriptor_idProduct(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->idProduct);
	return 1;
}

static int luausb_get_device_descriptor_bcdDevice(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bcdDevice);
	return 1;
}

static int luausb_get_device_descriptor_iManufacturer(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->iManufacturer);
	return 1;
}

static int luausb_get_device_descriptor_iProduct(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->iProduct);
	return 1;
}

static int luausb_get_device_descriptor_iSerialNumber(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->iSerialNumber);
	return 1;
}

static int luausb_get_device_descriptor_bNumConfigurations(lua_State* L)
{
	struct libusb_device_descriptor* udata;
	udata = luausb_to_device_descriptor(L, 1);
	lua_pushnumber(L, udata->bNumConfigurations);
	return 1;
}

static struct luaL_Reg libusb_device_descriptor__getters[] = {
	{"bLength", luausb_get_device_descriptor_bLength},
	{"bDescriptorType", luausb_get_device_descriptor_bDescriptorType},
	{"bcdUSB", luausb_get_device_descriptor_bcdUSB},
	{"bDeviceClass", luausb_get_device_descriptor_bDeviceClass},
	{"bDeviceSubClass", luausb_get_device_descriptor_bDeviceSubClass},
	{"bDeviceProtocol", luausb_get_device_descriptor_bDeviceProtocol},
	{"bMaxPacketSize0", luausb_get_device_descriptor_bMaxPacketSize0},
	{"idVendor", luausb_get_device_descriptor_idVendor},
	{"idProduct", luausb_get_device_descriptor_idProduct},
	{"bcdDevice", luausb_get_device_descriptor_bcdDevice},
	{"iManufacturer", luausb_get_device_descriptor_iManufacturer},
	{"iProduct", luausb_get_device_descriptor_iProduct},
	{"iSerialNumber", luausb_get_device_descriptor_iSerialNumber},
	{"bNumConfigurations", luausb_get_device_descriptor_bNumConfigurations},
	{0, 0},
};
static struct luaL_Reg libusb_device_descriptor__metamethods[] = {
	{"__index", luausb_generic_index},
	{"__tostring", luausb_generic_tostring},
	{0, 0},
};
extern struct luaL_Reg libusb_device_descriptor__methods[];

struct libusb_endpoint_descriptor* luausb_to_endpoint_descriptor(lua_State* L, int index)
{
	struct libusb_endpoint_descriptor* udata;
	udata = *(struct libusb_endpoint_descriptor**)lua_touserdata(L, index);
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

struct libusb_endpoint_descriptor* luausb_check_endpoint_descriptor(lua_State* L, int index)
{
	struct libusb_endpoint_descriptor* udata;
	udata = *(struct libusb_endpoint_descriptor**)luaL_checkudata(L, index, "struct libusb_endpoint_descriptor");
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

void luausb_push_endpoint_descriptor(lua_State* L, const struct libusb_endpoint_descriptor* value, int owner)
{
	if (owner < 0) owner = lua_gettop(L) + 1 + owner;
	const struct libusb_endpoint_descriptor** udata;
	udata = (const struct libusb_endpoint_descriptor**)lua_newuserdata(L, sizeof(struct libusb_endpoint_descriptor*));
	*udata = value;
	luaL_getmetatable(L, "struct libusb_endpoint_descriptor");
	lua_setmetatable(L, -2);
	if (owner != 0)
	{
		lua_createtable(L, 1, 0);
		lua_pushvalue(L, owner);
		lua_rawseti(L, -2, 1);
		setuservalue(L, -2);
	}
}

static int luausb_get_endpoint_descriptor_bLength(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	lua_pushnumber(L, udata->bLength);
	return 1;
}

static int luausb_get_endpoint_descriptor_bDescriptorType(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	lua_pushnumber(L, udata->bDescriptorType);
	return 1;
}

static int luausb_get_endpoint_descriptor_bEndpointAddress(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	lua_pushnumber(L, udata->bEndpointAddress);
	return 1;
}

static int luausb_get_endpoint_descriptor_bmAttributes(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	lua_pushnumber(L, udata->bmAttributes);
	return 1;
}

static int luausb_get_endpoint_descriptor_wMaxPacketSize(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	lua_pushnumber(L, udata->wMaxPacketSize);
	return 1;
}

static int luausb_get_endpoint_descriptor_bInterval(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	lua_pushnumber(L, udata->bInterval);
	return 1;
}

static int luausb_get_endpoint_descriptor_bRefresh(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	lua_pushnumber(L, udata->bRefresh);
	return 1;
}

static int luausb_get_endpoint_descriptor_bSynchAddress(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	lua_pushnumber(L, udata->bSynchAddress);
	return 1;
}

static int luausb_get_endpoint_descriptor_extra(lua_State* L)
{
	struct libusb_endpoint_descriptor* udata;
	udata = luausb_to_endpoint_descriptor(L, 1);
	if (udata->extra)
	{
		lua_pushlstring(L, (const char*)udata->extra, udata->extra_length);
	}
	else
		lua_pushnil(L);
	return 1;
}

static struct luaL_Reg libusb_endpoint_descriptor__getters[] = {
	{"bLength", luausb_get_endpoint_descriptor_bLength},
	{"bDescriptorType", luausb_get_endpoint_descriptor_bDescriptorType},
	{"bEndpointAddress", luausb_get_endpoint_descriptor_bEndpointAddress},
	{"bmAttributes", luausb_get_endpoint_descriptor_bmAttributes},
	{"wMaxPacketSize", luausb_get_endpoint_descriptor_wMaxPacketSize},
	{"bInterval", luausb_get_endpoint_descriptor_bInterval},
	{"bRefresh", luausb_get_endpoint_descriptor_bRefresh},
	{"bSynchAddress", luausb_get_endpoint_descriptor_bSynchAddress},
	{"extra", luausb_get_endpoint_descriptor_extra},
	{0, 0},
};
static struct luaL_Reg libusb_endpoint_descriptor__metamethods[] = {
	{"__index", luausb_generic_index},
	{"__tostring", luausb_generic_tostring},
	{0, 0},
};
extern struct luaL_Reg libusb_endpoint_descriptor__methods[];

struct libusb_interface_descriptor* luausb_to_interface_descriptor(lua_State* L, int index)
{
	struct libusb_interface_descriptor* udata;
	udata = *(struct libusb_interface_descriptor**)lua_touserdata(L, index);
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

struct libusb_interface_descriptor* luausb_check_interface_descriptor(lua_State* L, int index)
{
	struct libusb_interface_descriptor* udata;
	udata = *(struct libusb_interface_descriptor**)luaL_checkudata(L, index, "struct libusb_interface_descriptor");
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

void luausb_push_interface_descriptor(lua_State* L, const struct libusb_interface_descriptor* value, int owner)
{
	if (owner < 0) owner = lua_gettop(L) + 1 + owner;
	const struct libusb_interface_descriptor** udata;
	udata = (const struct libusb_interface_descriptor**)lua_newuserdata(L, sizeof(struct libusb_interface_descriptor*));
	*udata = value;
	luaL_getmetatable(L, "struct libusb_interface_descriptor");
	lua_setmetatable(L, -2);
	if (owner != 0)
	{
		lua_createtable(L, 1, 0);
		lua_pushvalue(L, owner);
		lua_rawseti(L, -2, 1);
		setuservalue(L, -2);
	}
}

static int luausb_get_interface_descriptor_bLength(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->bLength);
	return 1;
}

static int luausb_get_interface_descriptor_bDescriptorType(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->bDescriptorType);
	return 1;
}

static int luausb_get_interface_descriptor_bInterfaceNumber(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->bInterfaceNumber);
	return 1;
}

static int luausb_get_interface_descriptor_bAlternateSetting(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->bAlternateSetting);
	return 1;
}

static int luausb_get_interface_descriptor_bNumEndpoints(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->bNumEndpoints);
	return 1;
}

static int luausb_get_interface_descriptor_bInterfaceClass(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->bInterfaceClass);
	return 1;
}

static int luausb_get_interface_descriptor_bInterfaceSubClass(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->bInterfaceSubClass);
	return 1;
}

static int luausb_get_interface_descriptor_bInterfaceProtocol(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->bInterfaceProtocol);
	return 1;
}

static int luausb_get_interface_descriptor_iInterface(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_pushnumber(L, udata->iInterface);
	return 1;
}

int luausb_get_interface_descriptor_endpoint(lua_State* L);

static int luausb_get_interface_descriptor_extra(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	udata = luausb_to_interface_descriptor(L, 1);
	if (udata->extra)
	{
		lua_pushlstring(L, (const char*)udata->extra, udata->extra_length);
	}
	else
		lua_pushnil(L);
	return 1;
}

static struct luaL_Reg libusb_interface_descriptor__getters[] = {
	{"bLength", luausb_get_interface_descriptor_bLength},
	{"bDescriptorType", luausb_get_interface_descriptor_bDescriptorType},
	{"bInterfaceNumber", luausb_get_interface_descriptor_bInterfaceNumber},
	{"bAlternateSetting", luausb_get_interface_descriptor_bAlternateSetting},
	{"bNumEndpoints", luausb_get_interface_descriptor_bNumEndpoints},
	{"bInterfaceClass", luausb_get_interface_descriptor_bInterfaceClass},
	{"bInterfaceSubClass", luausb_get_interface_descriptor_bInterfaceSubClass},
	{"bInterfaceProtocol", luausb_get_interface_descriptor_bInterfaceProtocol},
	{"iInterface", luausb_get_interface_descriptor_iInterface},
	{"endpoint", luausb_get_interface_descriptor_endpoint},
	{"extra", luausb_get_interface_descriptor_extra},
	{0, 0},
};
static struct luaL_Reg libusb_interface_descriptor__metamethods[] = {
	{"__index", luausb_generic_index},
	{"__tostring", luausb_generic_tostring},
	{0, 0},
};
extern struct luaL_Reg libusb_interface_descriptor__methods[];

struct libusb_interface* luausb_to_interface(lua_State* L, int index)
{
	struct libusb_interface* udata;
	udata = *(struct libusb_interface**)lua_touserdata(L, index);
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

struct libusb_interface* luausb_check_interface(lua_State* L, int index)
{
	struct libusb_interface* udata;
	udata = *(struct libusb_interface**)luaL_checkudata(L, index, "struct libusb_interface");
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

void luausb_push_interface(lua_State* L, const struct libusb_interface* value, int owner)
{
	if (owner < 0) owner = lua_gettop(L) + 1 + owner;
	const struct libusb_interface** udata;
	udata = (const struct libusb_interface**)lua_newuserdata(L, sizeof(struct libusb_interface*));
	*udata = value;
	luaL_getmetatable(L, "struct libusb_interface");
	lua_setmetatable(L, -2);
	if (owner != 0)
	{
		lua_createtable(L, 1, 0);
		lua_pushvalue(L, owner);
		lua_rawseti(L, -2, 1);
		setuservalue(L, -2);
	}
}

int luausb_get_interface_altsetting(lua_State* L);

static int luausb_get_interface_num_altsetting(lua_State* L)
{
	struct libusb_interface* udata;
	udata = luausb_to_interface(L, 1);
	lua_pushnumber(L, udata->num_altsetting);
	return 1;
}

static struct luaL_Reg libusb_interface__getters[] = {
	{"altsetting", luausb_get_interface_altsetting},
	{"num_altsetting", luausb_get_interface_num_altsetting},
	{0, 0},
};
static struct luaL_Reg libusb_interface__metamethods[] = {
	{"__index", luausb_generic_index},
	{"__tostring", luausb_generic_tostring},
	{0, 0},
};
extern struct luaL_Reg libusb_interface__methods[];

struct libusb_config_descriptor* luausb_to_config_descriptor(lua_State* L, int index)
{
	struct libusb_config_descriptor* udata;
	udata = *(struct libusb_config_descriptor**)lua_touserdata(L, index);
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

struct libusb_config_descriptor* luausb_check_config_descriptor(lua_State* L, int index)
{
	struct libusb_config_descriptor* udata;
	udata = *(struct libusb_config_descriptor**)luaL_checkudata(L, index, "struct libusb_config_descriptor");
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

void luausb_push_config_descriptor(lua_State* L, const struct libusb_config_descriptor* value, int owner)
{
	if (owner < 0) owner = lua_gettop(L) + 1 + owner;
	const struct libusb_config_descriptor** udata;
	udata = (const struct libusb_config_descriptor**)lua_newuserdata(L, sizeof(struct libusb_config_descriptor*));
	*udata = value;
	luaL_getmetatable(L, "struct libusb_config_descriptor");
	lua_setmetatable(L, -2);
	if (owner != 0)
	{
		lua_createtable(L, 1, 0);
		lua_pushvalue(L, owner);
		lua_rawseti(L, -2, 1);
		setuservalue(L, -2);
	}
}

static int luausb_get_config_descriptor_bLength(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	lua_pushnumber(L, udata->bLength);
	return 1;
}

static int luausb_get_config_descriptor_bDescriptorType(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	lua_pushnumber(L, udata->bDescriptorType);
	return 1;
}

static int luausb_get_config_descriptor_wTotalLength(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	lua_pushnumber(L, udata->wTotalLength);
	return 1;
}

static int luausb_get_config_descriptor_bNumInterfaces(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	lua_pushnumber(L, udata->bNumInterfaces);
	return 1;
}

static int luausb_get_config_descriptor_bConfigurationValue(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	lua_pushnumber(L, udata->bConfigurationValue);
	return 1;
}

static int luausb_get_config_descriptor_iConfiguration(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	lua_pushnumber(L, udata->iConfiguration);
	return 1;
}

static int luausb_get_config_descriptor_bmAttributes(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	lua_pushnumber(L, udata->bmAttributes);
	return 1;
}

static int luausb_get_config_descriptor_MaxPower(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	lua_pushnumber(L, udata->MaxPower);
	return 1;
}

int luausb_get_config_descriptor_interface(lua_State* L);

static int luausb_get_config_descriptor_extra(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	udata = luausb_to_config_descriptor(L, 1);
	if (udata->extra)
	{
		lua_pushlstring(L, (const char*)udata->extra, udata->extra_length);
	}
	else
		lua_pushnil(L);
	return 1;
}

static struct luaL_Reg libusb_config_descriptor__getters[] = {
	{"bLength", luausb_get_config_descriptor_bLength},
	{"bDescriptorType", luausb_get_config_descriptor_bDescriptorType},
	{"wTotalLength", luausb_get_config_descriptor_wTotalLength},
	{"bNumInterfaces", luausb_get_config_descriptor_bNumInterfaces},
	{"bConfigurationValue", luausb_get_config_descriptor_bConfigurationValue},
	{"iConfiguration", luausb_get_config_descriptor_iConfiguration},
	{"bmAttributes", luausb_get_config_descriptor_bmAttributes},
	{"MaxPower", luausb_get_config_descriptor_MaxPower},
	{"interface", luausb_get_config_descriptor_interface},
	{"extra", luausb_get_config_descriptor_extra},
	{0, 0},
};
int luausb_config_descriptor_gc(lua_State* L);
static struct luaL_Reg libusb_config_descriptor__metamethods[] = {
	{"__index", luausb_generic_index},
	{"__tostring", luausb_generic_tostring},
	{"__gc", luausb_config_descriptor_gc},
	{0, 0},
};
extern struct luaL_Reg libusb_config_descriptor__methods[];

struct libusb_device* luausb_to_device(lua_State* L, int index)
{
	struct libusb_device* udata;
	udata = *(struct libusb_device**)lua_touserdata(L, index);
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

struct libusb_device* luausb_check_device(lua_State* L, int index)
{
	struct libusb_device* udata;
	udata = *(struct libusb_device**)luaL_checkudata(L, index, "struct libusb_device");
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

void luausb_push_device(lua_State* L, const struct libusb_device* value, int owner)
{
	if (owner < 0) owner = lua_gettop(L) + 1 + owner;
	const struct libusb_device** udata;
	udata = (const struct libusb_device**)lua_newuserdata(L, sizeof(struct libusb_device*));
	*udata = value;
	luaL_getmetatable(L, "struct libusb_device");
	lua_setmetatable(L, -2);
	if (owner != 0)
	{
		lua_createtable(L, 1, 0);
		lua_pushvalue(L, owner);
		lua_rawseti(L, -2, 1);
		setuservalue(L, -2);
	}
}

static struct luaL_Reg libusb_device__getters[] = {
	{0, 0},
};
int luausb_device_gc(lua_State* L);
static struct luaL_Reg libusb_device__metamethods[] = {
	{"__index", luausb_generic_index},
	{"__tostring", luausb_generic_tostring},
	{"__gc", luausb_device_gc},
	{0, 0},
};
extern struct luaL_Reg libusb_device__methods[];

struct libusb_device_handle* luausb_to_device_handle(lua_State* L, int index)
{
	struct libusb_device_handle* udata;
	udata = *(struct libusb_device_handle**)lua_touserdata(L, index);
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

struct libusb_device_handle* luausb_check_device_handle(lua_State* L, int index)
{
	struct libusb_device_handle* udata;
	udata = *(struct libusb_device_handle**)luaL_checkudata(L, index, "struct libusb_device_handle");
	if (!udata)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return udata;
}

void luausb_push_device_handle(lua_State* L, const struct libusb_device_handle* value, int owner)
{
	if (owner < 0) owner = lua_gettop(L) + 1 + owner;
	const struct libusb_device_handle** udata;
	udata = (const struct libusb_device_handle**)lua_newuserdata(L, sizeof(struct libusb_device_handle*));
	*udata = value;
	luaL_getmetatable(L, "struct libusb_device_handle");
	lua_setmetatable(L, -2);
	if (owner != 0)
	{
		lua_createtable(L, 1, 0);
		lua_pushvalue(L, owner);
		lua_rawseti(L, -2, 1);
		setuservalue(L, -2);
	}
}

static struct luaL_Reg libusb_device_handle__getters[] = {
	{0, 0},
};
int luausb_device_handle_gc(lua_State* L);
static struct luaL_Reg libusb_device_handle__metamethods[] = {
	{"__index", luausb_generic_index},
	{"__tostring", luausb_generic_tostring},
	{"__gc", luausb_device_handle_gc},
	{0, 0},
};
extern struct luaL_Reg libusb_device_handle__methods[];

void luausb_init_structs(lua_State* L)
{
	/* device_descriptor */
	luaL_newmetatable(L, "struct libusb_device_descriptor");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device_descriptor__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device_descriptor__methods, 1);
	lua_setfield(L, -2, "methods");
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device_descriptor__getters, 1);
	lua_setfield(L, -2, "getters");
	lua_pushliteral(L, "device_descriptor");
	lua_setfield(L, -2, "typename");
	lua_pop(L, 1);
	/* endpoint_descriptor */
	luaL_newmetatable(L, "struct libusb_endpoint_descriptor");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_endpoint_descriptor__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_endpoint_descriptor__methods, 1);
	lua_setfield(L, -2, "methods");
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_endpoint_descriptor__getters, 1);
	lua_setfield(L, -2, "getters");
	lua_pushliteral(L, "endpoint_descriptor");
	lua_setfield(L, -2, "typename");
	lua_pop(L, 1);
	/* interface_descriptor */
	luaL_newmetatable(L, "struct libusb_interface_descriptor");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_interface_descriptor__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_interface_descriptor__methods, 1);
	lua_setfield(L, -2, "methods");
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_interface_descriptor__getters, 1);
	lua_setfield(L, -2, "getters");
	lua_pushliteral(L, "interface_descriptor");
	lua_setfield(L, -2, "typename");
	lua_pop(L, 1);
	/* interface */
	luaL_newmetatable(L, "struct libusb_interface");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_interface__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_interface__methods, 1);
	lua_setfield(L, -2, "methods");
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_interface__getters, 1);
	lua_setfield(L, -2, "getters");
	lua_pushliteral(L, "interface");
	lua_setfield(L, -2, "typename");
	lua_pop(L, 1);
	/* config_descriptor */
	luaL_newmetatable(L, "struct libusb_config_descriptor");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_config_descriptor__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_config_descriptor__methods, 1);
	lua_setfield(L, -2, "methods");
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_config_descriptor__getters, 1);
	lua_setfield(L, -2, "getters");
	lua_pushliteral(L, "config_descriptor");
	lua_setfield(L, -2, "typename");
	lua_pop(L, 1);
	/* device */
	luaL_newmetatable(L, "struct libusb_device");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device__methods, 1);
	lua_setfield(L, -2, "methods");
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device__getters, 1);
	lua_setfield(L, -2, "getters");
	lua_pushliteral(L, "device");
	lua_setfield(L, -2, "typename");
	lua_pop(L, 1);
	/* device_handle */
	luaL_newmetatable(L, "struct libusb_device_handle");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device_handle__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device_handle__methods, 1);
	lua_setfield(L, -2, "methods");
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_device_handle__getters, 1);
	lua_setfield(L, -2, "getters");
	lua_pushliteral(L, "device_handle");
	lua_setfield(L, -2, "typename");
	lua_pop(L, 1);
}
