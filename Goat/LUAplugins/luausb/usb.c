#include <lua.h>
#include <lauxlib.h>
#ifdef WIN32
#include "include/libusb.h"
#else
#include <libusb-1.0/libusb.h>
#endif
#include <string.h>
#include "compat.h"
#include "enums.h"
#include "structs.h"

/****************************************************************************/

static int lua__usberror(lua_State* L, int usberror)
{
	lua_pushnil(L);
#ifdef WIN32
	lua_pushstring(L, libusb_strerror(usberror));
#else
	switch (usberror)
	{
	case LIBUSB_ERROR_IO: lua_pushstring(L, "io"); break;
	case LIBUSB_ERROR_INVALID_PARAM: lua_pushstring(L, "invalid_param"); break;
	case LIBUSB_ERROR_ACCESS: lua_pushstring(L, "access"); break;
	case LIBUSB_ERROR_NO_DEVICE: lua_pushstring(L, "no_device"); break;
	case LIBUSB_ERROR_NOT_FOUND: lua_pushstring(L, "not_found"); break;
	case LIBUSB_ERROR_BUSY: lua_pushstring(L, "busy"); break;
	case LIBUSB_ERROR_TIMEOUT: lua_pushstring(L, "timeout"); break;
	case LIBUSB_ERROR_OVERFLOW: lua_pushstring(L, "overflow"); break;
	case LIBUSB_ERROR_PIPE: lua_pushstring(L, "pipe"); break;
	case LIBUSB_ERROR_INTERRUPTED: lua_pushstring(L, "interrupted"); break;
	case LIBUSB_ERROR_NO_MEM: lua_pushstring(L, "no_mem"); break;
	case LIBUSB_ERROR_NOT_SUPPORTED: lua_pushstring(L, "not_supported"); break;
	default:
	case LIBUSB_ERROR_OTHER: lua_pushstring(L, "other"); break;
	}
#endif
	lua_pushnumber(L, usberror);
	return 3;
}

/****************************************************************************/

libusb_context* luausb_check_context(lua_State* L, int index)
{
	libusb_context* ctx;
	ctx = *(libusb_context**)luaL_checkudata(L, index, "libusb_context");
	if (!ctx)
	{
		luaL_argerror(L, index, "context has already been deinitialized");
		return NULL;
	}
	return ctx;
}

void luausb_push_context(lua_State* L, libusb_context* value)
{
	libusb_context** udata;
	udata = (libusb_context**)lua_newuserdata(L, sizeof(libusb_context*));
	*udata = value;
	luaL_getmetatable(L, "libusb_context");
	lua_setmetatable(L, -2);
}

/****************************************************************************/

#define BINDING(f) static int lua__libusb_##f(lua_State* L)
#define BIND(f) {#f, lua__libusb_##f},

BINDING(init)
{
	libusb_context* context;
	int result;
	
	result = libusb_init(&context);
	if (result != LIBUSB_SUCCESS)
		return lua__usberror(L, result);
	
	luausb_push_context(L, context);
	return 1;
}

int luausb_context_gc(lua_State* L)
{
	libusb_context** ctx;
	
	ctx = (libusb_context**)lua_touserdata(L, 1);
	if (*ctx)
	{
		libusb_exit(*ctx);
		*ctx = 0;
	}
	return 0;
}

BINDING(exit)
{
	luausb_check_context(L, 1);
	return luausb_context_gc(L);
}

BINDING(get_device_list)
{
	libusb_context* ctx;
	libusb_device** list;
	ssize_t result, i;
	
	ctx = luausb_check_context(L, 1);
	
	result = libusb_get_device_list(ctx, &list);
	if (result < 0)
		return lua__usberror(L, (int)result);
	
	lua_createtable(L, (int)result, 0);
	for (i=0; i<result; ++i)
	{
		luausb_push_device(L, list[i], 1);
		lua_rawseti(L, -2, (int)i+1);
	}
	libusb_free_device_list(list, 0);	
	return 1;
}

BINDING(get_bus_number)
{
	libusb_device* dev;
	uint8_t result;
	
	dev = luausb_check_device(L, 1);
	
	result = libusb_get_bus_number(dev);
	
	lua_pushnumber(L, result);
	return 1;
}

BINDING(get_device_address)
{
	libusb_device* dev;
	uint8_t result;
	
	dev = luausb_check_device(L, 1);
	
	result = libusb_get_device_address(dev);
	
	lua_pushnumber(L, result);
	return 1;
}

BINDING(get_max_packet_size)
{
	libusb_device* dev;
	unsigned char endpoint;
	int result;
	
	dev = luausb_check_device(L, 1);
	endpoint = (unsigned char)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_get_max_packet_size(dev, endpoint);
	if (result < 0)
		return lua__usberror(L, result);
	
	lua_pushnumber(L, result);
	return 1;
}

BINDING(get_max_iso_packet_size)
{
	libusb_device* dev;
	unsigned char endpoint;
	int result;
	
	dev = luausb_check_device(L, 1);
	endpoint = (unsigned char)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_get_max_iso_packet_size(dev, endpoint);
	if (result < 0)
		return lua__usberror(L, result);
	
	lua_pushnumber(L, result);
	return 1;
}

int luausb_device_gc(lua_State* L)
{
	libusb_device** dev;
	
	dev = (libusb_device**)lua_touserdata(L, 1);
	if (*dev)
	{
		libusb_unref_device(*dev);
		*dev = 0;
	}
	return 0;
}

BINDING(unref_device)
{
	luausb_check_device(L, 1);
	return luausb_device_gc(L);
}

BINDING(open)
{
	libusb_device* dev;
	libusb_device_handle* handle;
	int result;
	
	dev = luausb_check_device(L, 1);
	
	result = libusb_open(dev, &handle);
	if (result != 0)
		return lua__usberror(L, result);
	
	luausb_push_device_handle(L, handle, 1);
	return 1;
}

BINDING(open_device_with_vid_pid)
{
	libusb_context* ctx;
	uint16_t vendor_id;
	uint16_t product_id;
	libusb_device_handle* result;
	
	ctx = luausb_check_context(L, 1);
	vendor_id = (uint16_t)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	product_id = (uint16_t)luaL_checknumber(L, 3); /* :FIXME: handle overflow */
	
	result = libusb_open_device_with_vid_pid(ctx, vendor_id, product_id);
	
	if (result)
		luausb_push_device_handle(L, result, 1);
	else
		lua_pushnil(L);
	return 1;
}

int luausb_device_handle_gc(lua_State* L)
{
	libusb_device_handle** dev_handle;
	
	dev_handle = (libusb_device_handle**)lua_touserdata(L, 1);
	if (*dev_handle)
	{
		libusb_close(*dev_handle);
		*dev_handle = 0;
	}
	return 0;
}

BINDING(close)
{
	luausb_check_device_handle(L, 1);
	return luausb_device_handle_gc(L);
}

BINDING(get_device)
{
	libusb_device_handle* dev_handle;
	libusb_device* result;
	
	dev_handle = luausb_check_device_handle(L, 1);
	
	result = libusb_get_device(dev_handle);
	
	libusb_ref_device(result);
	luausb_push_device(L, result, 1);
	return 1;
}

BINDING(get_configuration)
{
	libusb_device_handle* dev;
	int config;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	
	result = libusb_get_configuration(dev, &config);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushnumber(L, config);
	return 1;
}

BINDING(set_configuration)
{
	libusb_device_handle* dev;
	int configuration;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	configuration = (int)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_set_configuration(dev, configuration);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, 1);
	return 1;
}

BINDING(claim_interface)
{
	libusb_device_handle* dev;
	int interface_number;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	interface_number = (int)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_claim_interface(dev, interface_number);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, 1);
	return 1;
}

BINDING(release_interface)
{
	libusb_device_handle* dev;
	int interface_number;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	interface_number = (int)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_release_interface(dev, interface_number);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, 1);
	return 1;
}

BINDING(set_interface_alt_setting)
{
	libusb_device_handle* dev;
	int interface_number;
	int alternate_setting;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	interface_number = (int)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	alternate_setting = (int)luaL_checknumber(L, 3); /* :FIXME: handle overflow */
	
	result = libusb_set_interface_alt_setting(dev, interface_number, alternate_setting);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, 1);
	return 1;
}

BINDING(clear_halt)
{
	libusb_device_handle* dev;
	unsigned char endpoint;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	endpoint = (unsigned char)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_clear_halt(dev, endpoint);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, 1);
	return 1;
}

BINDING(reset_device)
{
	libusb_device_handle* dev;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	
	result = libusb_reset_device(dev);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, 1);
	return 1;
}

BINDING(kernel_driver_active)
{
	libusb_device_handle* dev;
	int interface;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	interface = (int)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_kernel_driver_active(dev, interface);
	if (result < 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, result);
	return 1;
}

BINDING(detach_kernel_driver)
{
	libusb_device_handle* dev;
	int interface;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	interface = (int)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_detach_kernel_driver(dev, interface);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, 1);
	return 1;
}

BINDING(attach_kernel_driver)
{
	libusb_device_handle* dev;
	int interface;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	interface = (int)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_attach_kernel_driver(dev, interface);
	if (result != 0)
		return lua__usberror(L, result);
	
	lua_pushboolean(L, 1);
	return 1;
}

BINDING(get_device_descriptor)
{
	libusb_device* dev;
	struct libusb_device_descriptor* desc;
	int result;
	
	dev = luausb_check_device(L, 1);
	
	desc = (struct libusb_device_descriptor*)lua_newuserdata(L, sizeof(struct libusb_device_descriptor));
	
	result = libusb_get_device_descriptor(dev, desc);
	if (result != 0)
		return lua__usberror(L, result);
	
	luausb_push_device_descriptor(L, desc, -1);
	return 1;
}

BINDING(get_active_config_descriptor)
{
	libusb_device* dev;
	struct libusb_config_descriptor* config;
	int result;
	
	dev = luausb_check_device(L, 1);
	
	result = libusb_get_active_config_descriptor(dev, &config);
	if (result != 0)
		return lua__usberror(L, result);
	
	luausb_push_config_descriptor(L, config, 1);
	return 1;
}

BINDING(get_config_descriptor)
{
	libusb_device* dev;
	uint8_t config_index;
	struct libusb_config_descriptor* config;
	int result;
	
	dev = luausb_check_device(L, 1);
	config_index = (uint8_t)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_get_config_descriptor(dev, config_index, &config);
	if (result != 0)
		return lua__usberror(L, result);
	
	luausb_push_config_descriptor(L, config, 1);
	return 1;
}

BINDING(get_config_descriptor_by_value)
{
	libusb_device* dev;
	uint8_t bConfigurationValue;
	struct libusb_config_descriptor* config;
	int result;
	
	dev = luausb_check_device(L, 1);
	bConfigurationValue = (uint8_t)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	
	result = libusb_get_config_descriptor_by_value(dev, bConfigurationValue, &config);
	if (result != 0)
		return lua__usberror(L, result);
	
	luausb_push_config_descriptor(L, config, 1);
	return 1;
}

int luausb_config_descriptor_gc(lua_State* L)
{
	struct libusb_config_descriptor** config;
	
	config = (struct libusb_config_descriptor**)lua_touserdata(L, 1);
	if (*config)
	{
		libusb_free_config_descriptor(*config);
		*config = 0;
	}
	return 0;
}

BINDING(free_config_descriptor)
{
	if (!luausb_check_config_descriptor(L, 1))
		return luaL_error(L, "trying to use a freed libusb_config_descriptor");
	return luausb_config_descriptor_gc(L);
}

BINDING(get_string_descriptor_ascii)
{
	libusb_device_handle* dev;
	uint8_t desc_index;
	unsigned char* data;
	int length;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	desc_index = (uint8_t)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	length = (int)luaL_checknumber(L, 3); /* :FIXME: handle overflow */
	
	data = lua_newuserdata(L, length);
	
	result = libusb_get_string_descriptor_ascii(dev, desc_index, data, length);
	if (result < 0)
		return lua__usberror(L, result);
	
	lua_pushlstring(L, (char*)data, result);
	return 1;
}

BINDING(get_descriptor)
{
	libusb_device_handle* dev;
	uint8_t desc_type;
	uint8_t desc_index;
	unsigned char* data;
	int length;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	desc_type = (uint8_t)luausb_check_descriptor_type(L, 2);
	desc_index = (uint8_t)luaL_checknumber(L, 3); /* :FIXME: handle overflow */
//	length = (int)luaL_checknumber(L, 4); /* :FIXME: handle overflow */
	
	switch (desc_type)
	{
	case LIBUSB_DT_DEVICE:
//		length = sizeof(struct libusb_device_descriptor);
//		break;
	case LIBUSB_DT_CONFIG:
//		length = sizeof(struct libusb_config_descriptor);
//		break;
	case LIBUSB_DT_INTERFACE:
//		length = sizeof(struct libusb_interface_descriptor);
//		break;
	case LIBUSB_DT_ENDPOINT:
//		length = sizeof(struct libusb_endpoint_descriptor);
//		break;
	case LIBUSB_DT_STRING:
	case LIBUSB_DT_HID:
	case LIBUSB_DT_REPORT:
	case LIBUSB_DT_PHYSICAL:
	case LIBUSB_DT_HUB:
	default:
		length = (int)luaL_checknumber(L, 4); /* :FIXME: handle overflow */
	}
	
	data = lua_newuserdata(L, length);
	
	result = libusb_get_descriptor(dev, desc_type, desc_index, data, length);
	if (result < 0)
		return lua__usberror(L, result);
	
	switch (desc_type)
	{
	case LIBUSB_DT_DEVICE:
		luausb_push_device_descriptor(L, (struct libusb_device_descriptor*)data, 1);
		break;
	case LIBUSB_DT_CONFIG:
		luausb_push_config_descriptor(L, (struct libusb_config_descriptor*)data, 1);
		break;
	case LIBUSB_DT_INTERFACE:
		luausb_push_interface_descriptor(L, (struct libusb_interface_descriptor*)data, 1);
		break;
	case LIBUSB_DT_ENDPOINT:
		luausb_push_endpoint_descriptor(L, (struct libusb_endpoint_descriptor*)data, 1);
		break;
	case LIBUSB_DT_STRING:
	case LIBUSB_DT_HID:
	case LIBUSB_DT_REPORT:
	case LIBUSB_DT_PHYSICAL:
	case LIBUSB_DT_HUB:
	default:
		lua_pushlstring(L, (char*)data, result);
	}
	return 1;
}

BINDING(get_string_descriptor)
{
	libusb_device_handle* dev;
	uint8_t desc_index;
	uint16_t langid;
	unsigned char* data;
	int length;
	int result;
	
	dev = luausb_check_device_handle(L, 1);
	desc_index = (uint8_t)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	langid = (uint16_t)luaL_checknumber(L, 3); /* :FIXME: handle overflow */
	length = (int)luaL_checknumber(L, 4); /* :FIXME: handle overflow */
	
	data = lua_newuserdata(L, length);
	
	result = libusb_get_string_descriptor(dev, desc_index, langid, data, length);
	if (result < 0)
		return lua__usberror(L, result);
	
	/* :TODO: convert to utf-8 */
	lua_pushlstring(L, (char*)data, length);
	return 1;
}

BINDING(control_transfer)
{
	struct libusb_device_handle* dev_handle;
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	unsigned char* data;
	uint16_t wLength;
	unsigned int timeout;
	int result;
	
	dev_handle = luausb_check_device_handle(L, 1);
	bmRequestType = (uint8_t)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	bRequest = (uint8_t)luaL_checknumber(L, 3); /* :FIXME: handle overflow */
	wValue = (uint16_t)luaL_checknumber(L, 4); /* :FIXME: handle overflow */
	wIndex = (uint16_t)luaL_checknumber(L, 5); /* :FIXME: handle overflow */
	if (bmRequestType & 0x80)
	{
		/* input request */
		luaL_checktype(L, 6, LUA_TNUMBER);
		wLength = (uint16_t)lua_tonumber(L, 6); /* :FIXME: handle overflow */
		data = (unsigned char*)lua_newuserdata(L, wLength);
	}
	else
	{
		/* output request */
		size_t len;
		const char* str;
		luaL_checktype(L, 6, LUA_TSTRING);
		str = lua_tolstring(L, 6, &len);
		wLength = (uint16_t)len; /* :FIXME: handle overflow */
		data = (unsigned char*)lua_newuserdata(L, wLength);
		memcpy(data, str, len);
	}
	timeout = (unsigned int)luaL_checknumber(L, 7); /* :FIXME: handle overflow */
	
	result = libusb_control_transfer(dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	if (result < 0)
		return lua__usberror(L, result);
	
	if (bmRequestType & 0x80)
	{
		/* input request */
		lua_pushlstring(L, (char*)data, result);
		return 1;
	}
	else
	{
		/* output request */
		lua_pushnumber(L, result);
		return 1;
	}
}

BINDING(bulk_transfer)
{
	struct libusb_device_handle* dev_handle;
	unsigned char endpoint;
	unsigned char* data;
	int length;
	int transferred;
	unsigned int timeout;
	int result;
	int i;
	
	dev_handle = luausb_check_device_handle(L, 1);
	endpoint = (unsigned char)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	if (endpoint & LIBUSB_ENDPOINT_IN)
	{
		/* in endpoint */
		length = (int)luaL_checknumber(L, 3); /* :FIXME: handle overflow */
		data = (unsigned char*)lua_newuserdata(L, length);
	}
	else
	{
		/* out endpoint */
		size_t len;
		const char* str;
		str = luaL_checklstring(L, 3, &len);
		length = (int)len; /* :FIXME: handle overflow */
		data = (unsigned char*)lua_newuserdata(L, length);
		memcpy(data, str, len);
	}
	timeout = (unsigned int)luaL_checknumber(L, 4); /* :FIXME: handle overflow */
	
	result = libusb_bulk_transfer(dev_handle, endpoint, data, length, &transferred, timeout);
	if (result < 0 && result != LIBUSB_ERROR_TIMEOUT)
		return lua__usberror(L, result);
	
	/* lua__usberror will push nil and error codes, replace the nil */
	i = lua__usberror(L, result);
	if (endpoint & LIBUSB_ENDPOINT_IN)
		/* in endpoint */
		lua_pushlstring(L, (char*)data, transferred);
	else
		/* out endpoint */
		lua_pushnumber(L, transferred);
	lua_replace(L, -1-i);
	return i;
}

BINDING(interrupt_transfer)
{
	struct libusb_device_handle* dev_handle;
	unsigned char endpoint;
	unsigned char* data;
	int length;
	int transferred;
	unsigned int timeout;
	int result;
	int i = 0;
	
	dev_handle = luausb_check_device_handle(L, 1);
	endpoint = (unsigned char)luaL_checknumber(L, 2); /* :FIXME: handle overflow */
	if (endpoint & LIBUSB_ENDPOINT_IN)
	{
		/* in endpoint */
		length = (int)luaL_checknumber(L, 3); /* :FIXME: handle overflow */
		data = (unsigned char*)lua_newuserdata(L, length);
	}
	else
	{
		/* out endpoint */
		size_t len;
		const char* str;
		str = luaL_checklstring(L, 3, &len);
		length = (int)len; /* :FIXME: handle overflow */
		data = (unsigned char*)lua_newuserdata(L, length);
		memcpy(data, str, len);
	}
	timeout = (unsigned int)luaL_checknumber(L, 4); /* :FIXME: handle overflow */
	
	result = libusb_interrupt_transfer(dev_handle, endpoint, data, length, &transferred, timeout);
	if (result < 0 && result != LIBUSB_ERROR_TIMEOUT)
		return lua__usberror(L, result);
	
	/* lua__usberror will push nil and error codes, replace the nil */
	if (result < 0)
		i = lua__usberror(L, result);
	if (endpoint & LIBUSB_ENDPOINT_IN)
		/* in endpoint */
		lua_pushlstring(L, (char*)data, transferred);
	else
		/* out endpoint */
		lua_pushnumber(L, transferred);
	if (result < 0)
	{
		lua_replace(L, -1-i);
		return i;
	}
	else
		return 1;
}

/****************************************************************************/

int luausb_get_interface_descriptor_endpoint(lua_State* L)
{
	struct libusb_interface_descriptor* udata;
	int i;
	udata = luausb_to_interface_descriptor(L, 1);
	lua_createtable(L, udata->bNumEndpoints, 0);
	for (i=0; i<udata->bNumEndpoints; ++i)
	{
		luausb_push_endpoint_descriptor(L, &udata->endpoint[i], 1);
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}

int luausb_get_interface_altsetting(lua_State* L)
{
	struct libusb_interface* udata;
	int i;
	udata = luausb_to_interface(L, 1);
	lua_createtable(L, udata->num_altsetting, 0);
	for (i=0; i<udata->num_altsetting; ++i)
	{
		luausb_push_interface_descriptor(L, &udata->altsetting[i], 1);
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}

int luausb_get_config_descriptor_interface(lua_State* L)
{
	struct libusb_config_descriptor* udata;
	uint8_t i;
	udata = luausb_to_config_descriptor(L, 1);
	lua_createtable(L, udata->bNumInterfaces, 0);
	for (i=0; i<udata->bNumInterfaces; ++i)
	{
		luausb_push_interface(L, &udata->interface[i], 1);
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}

/****************************************************************************/

static luaL_Reg functions[] = {
	BIND(init)
	{0, 0},
};

struct luaL_Reg libusb_context__metamethods[] = {
	{"__gc", luausb_context_gc},
	{0, 0},
};

struct luaL_Reg libusb_context__methods[] = {
	BIND(exit)
	BIND(get_device_list)
	BIND(open_device_with_vid_pid)
	{0, 0},
};
/*
struct luaL_Reg libusb_device__metamethods[] = {
	{"__gc", luausb_device_gc},
	{0, 0},
};
*/
struct luaL_Reg libusb_device__methods[] = {
	BIND(get_bus_number)
	BIND(get_device_address)
	BIND(get_max_packet_size)
	BIND(get_max_iso_packet_size)
	BIND(unref_device)
	BIND(open)
	BIND(get_device_descriptor)
	BIND(get_active_config_descriptor)
	BIND(get_config_descriptor)
	BIND(get_config_descriptor_by_value)
	{0, 0},
};
/*
struct luaL_Reg libusb_device_handle__metamethods[] = {
	{"__gc", luausb_device_handle_gc},
	{0, 0},
};
*/
struct luaL_Reg libusb_device_handle__methods[] = {
	BIND(close)
	BIND(get_device)
	BIND(get_configuration)
	BIND(set_configuration)
	BIND(claim_interface)
	BIND(release_interface)
	BIND(set_interface_alt_setting)
	BIND(clear_halt)
	BIND(reset_device)
	BIND(kernel_driver_active)
	BIND(detach_kernel_driver)
	BIND(attach_kernel_driver)
	BIND(get_string_descriptor_ascii)
	BIND(get_descriptor)
	BIND(get_string_descriptor)
	BIND(control_transfer)
	BIND(bulk_transfer)
	BIND(interrupt_transfer)
	{0, 0},
};
/*
struct luaL_Reg libusb_config_descriptor__metamethods[] = {
	{"__gc", luausb_config_descriptor_gc},
	{0, 0},
};
*/
struct luaL_Reg libusb_config_descriptor__methods[] = {
	BIND(free_config_descriptor)
	{0, 0},
};
/*
struct luaL_Reg libusb_interface__metamethods[] = {
	{0, 0},
};
*/
struct luaL_Reg libusb_interface__methods[] = {
	{0, 0},
};
/*
struct luaL_Reg libusb_device_descriptor__metamethods[] = {
	{0, 0},
};
*/
struct luaL_Reg libusb_device_descriptor__methods[] = {
	{0, 0},
};
/*
struct luaL_Reg libusb_interface_descriptor__metamethods[] = {
	{0, 0},
};
*/
struct luaL_Reg libusb_interface_descriptor__methods[] = {
	{0, 0},
};
/*
struct luaL_Reg libusb_endpoint_descriptor__metamethods[] = {
	{0, 0},
};
*/
struct luaL_Reg libusb_endpoint_descriptor__methods[] = {
	{0, 0},
};

/****************************************************************************/

LUAMOD_API int luaopen_module(lua_State* L)
{
	/* module */
#if LUA_VERSION_NUM==502
	lua_newtable(L);
#elif LUA_VERSION_NUM==501
	struct luaL_Reg empty[] = {{0,0}};
	luaL_register(L, lua_tostring(L, 1), empty);
#else
#error unsupported Lua version
#endif
	lua_replace(L, 1); /* code below assumes module is at index 1 */
	lua_settop(L, 1);
	/* init enums */
	luausb_init_enums(L);
	/* init structs */
	luausb_init_structs(L);
	/* context */
	luaL_newmetatable(L, "libusb_context");
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_context__metamethods, 1);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	setfuncs(L, libusb_context__methods, 1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
	/* module functions */
	lua_pushvalue(L, 1);
	setfuncs(L, functions, 1);
	return 1;
}

