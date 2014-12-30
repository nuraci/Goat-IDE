------------------------------------------------------------------------
-- Nunzio Raciti for Goat <raciti.nunzio@gmail.com>
--
-- This program is PUBLIC DOMAIN
------------------------------------------------------------------------

local pluginsDir = props['PluginsDir'] 
local binDir = props['BinDir']
local  PLAT_WIN = props['PLAT_WIN']

if  PLAT_WIN == "1" then
package.cpath = binDir .. "\\?.dll" 
package.path =  pluginsDir .. "\\libusb\\?.lua"
else
package.cpath = binDir .. "/?.so;" 
package.path =  pluginsDir .. "/libusb/?.lua"
end
 
usb = require "luausb"
require 'names'
require 'bit'

names.init(pluginsDir .. "/libusb/usb.ids")

verblevel=3
lua_report = true

local DFU_TIMEOUT  = 10000

local USB_DIR_OUT         = 0           -- to device
local USB_DIR_IN          = 0x80        -- to host

local USB_ENDPOINT_IN = USB_DIR_IN
local USB_ENDPOINT_OUT = USB_DIR_OUT

-- USB types, the second of three bRequestType fields
local USB_TYPE_MASK           = (0x03 * 2^5)
local USB_TYPE_STANDARD       = (0x00 * 2^5)
local USB_TYPE_CLASS          = (0x01 * 2^5)
local USB_TYPE_VENDOR         = (0x02 * 2^5)
local USB_TYPE_RESERVED       = (0x03 * 2^5)

-- USB recipients, the third of three bRequestType fields
local USB_RECIP_MASK          = 0x1f
local USB_RECIP_DEVICE        = 0x00
local USB_RECIP_INTERFACE     = 0x01
local USB_RECIP_ENDPOINT      = 0x02
local USB_RECIP_OTHER         = 0x03
-- From Wireless USB 1.0
local USB_RECIP_PORT          = 0x04
local USB_RECIP_RPIPE         = 0x05

-- Standard requests, for the bRequest field of a SETUP packet.
--
-- These are qualified by the bRequestType field, so that for example
-- TYPE_CLASS or TYPE_VENDOR specific feature flags could be retrieved
-- by a GET_STATUS request.
local USB_REQ_GET_STATUS          = 0x00
local USB_REQ_CLEAR_FEATURE       = 0x01
local USB_REQ_SET_FEATURE         = 0x03
local USB_REQ_SET_ADDRESS         = 0x05
local USB_REQ_GET_DESCRIPTOR      = 0x06
local USB_REQ_SET_DESCRIPTOR      = 0x07
local USB_REQ_GET_CONFIGURATION   = 0x08
local USB_REQ_SET_CONFIGURATION   = 0x09
local USB_REQ_GET_INTERFACE       = 0x0A
local USB_REQ_SET_INTERFACE       = 0x0B
local USB_REQ_SYNCH_FRAME         = 0x0C

local USB_REQ_SET_ENCRYPTION      = 0x0D    -- Wireless USB
local USB_REQ_GET_ENCRYPTION      = 0x0E
local USB_REQ_RPIPE_ABORT         = 0x0E
local USB_REQ_SET_HANDSHAKE       = 0x0F
local USB_REQ_RPIPE_RESET         = 0x0F
local USB_REQ_GET_HANDSHAKE       = 0x10
local USB_REQ_SET_CONNECTION      = 0x11
local USB_REQ_SET_SECURITY_DATA   = 0x12
local USB_REQ_GET_SECURITY_DATA   = 0x13
local USB_REQ_SET_WUSB_DATA       = 0x14
local USB_REQ_LOOPBACK_DATA_WRITE = 0x15
local USB_REQ_LOOPBACK_DATA_READ  = 0x16
local USB_REQ_SET_INTERFACE_DS    = 0x17

-- from USB 2.0 spec and updates
local USB_DT_DEVICE_QUALIFIER		= 0x06
local USB_DT_OTHER_SPEED_CONFIG		= 0x07
local USB_DT_OTG					= 0x09
local USB_DT_DEBUG					= 0x0a
local USB_DT_INTERFACE_ASSOCIATION	= 0x0b
local USB_DT_SECURITY				= 0x0c
local USB_DT_KEY					= 0x0d
local USB_DT_ENCRYPTION_TYPE		= 0x0e
local USB_DT_BOS					= 0x0f
local USB_DT_DEVICE_CAPABILITY		= 0x10
local USB_DT_WIRELESS_ENDPOINT_COMP	= 0x11
local USB_DT_WIRE_ADAPTER			= 0x21
local USB_DT_RPIPE					= 0x22

local USB_DT_RC_INTERFACE			= 0x23

-- Descriptor types ... USB 2.0 spec table 9.5
local USB_DT_DEVICE           = 0x01
local USB_DT_CONFIG           = 0x02
local USB_DT_STRING           = 0x03
local USB_DT_INTERFACE        = 0x04
local USB_DT_ENDPOINT         = 0x05
local USB_DT_DEVICE_QUALIFIER     = 0x06
local USB_DT_OTHER_SPEED_CONFIG   = 0x07
local USB_DT_INTERFACE_POWER      = 0x08
-- these are from a minor usb 2.0 revision (ECN)
local USB_DT_OTG          = 0x09
local USB_DT_DEBUG            = 0x0a
local USB_DT_INTERFACE_ASSOCIATION    = 0x0b
-- these are from the Wireless USB spec
local USB_DT_SECURITY         = 0x0c
local USB_DT_KEY          = 0x0d
local USB_DT_ENCRYPTION_TYPE      = 0x0e
local USB_DT_BOS          = 0x0f
local USB_DT_DEVICE_CAPABILITY    = 0x10
local USB_DT_WIRELESS_ENDPOINT_COMP   = 0x11
local USB_DT_WIRE_ADAPTER     = 0x21
local USB_DT_RPIPE            = 0x22
local USB_DT_CS_RADIO_CONTROL     = 0x23
-- From the USB 3.0 spec
local USB_DT_SS_ENDPOINT_COMP     = 0x30

-- Conventional codes for class-specific descriptors.  The convention is
-- defined in the USB "Common Class" Spec (3.11).  Individual class specs
-- are authoritative for their usage, not the "common class" writeup.
local USB_DT_CS_DEVICE        = bit.bor(USB_TYPE_CLASS, USB_DT_DEVICE)
local USB_DT_CS_CONFIG        = bit.bor(USB_TYPE_CLASS, USB_DT_CONFIG)
local USB_DT_CS_STRING        = bit.bor(USB_TYPE_CLASS, USB_DT_STRING)
local USB_DT_CS_INTERFACE     = bit.bor(USB_TYPE_CLASS, USB_DT_INTERFACE)
local USB_DT_CS_ENDPOINT      = bit.bor(USB_TYPE_CLASS, USB_DT_ENDPOINT)


-- Device and/or Interface Class codes
-- as found in bDeviceClass or bInterfaceClass
-- and defined by www.usb.org documents
local USB_CLASS_PER_INTERFACE     = 0   -- for DeviceClass
local USB_CLASS_AUDIO         = 1
local USB_CLASS_COMM          = 2
local USB_CLASS_HID           = 3
local USB_CLASS_PHYSICAL      = 5
local USB_CLASS_STILL_IMAGE       = 6
local USB_CLASS_PRINTER       = 7
local USB_CLASS_MASS_STORAGE      = 8
local USB_CLASS_HUB           = 9
local USB_CLASS_CDC_DATA      = 0x0a
local USB_CLASS_CSCID         = 0x0b    -- chip+ smart card
local USB_CLASS_CONTENT_SEC       = 0x0d    -- content security
local USB_CLASS_VIDEO         = 0x0e
local USB_CLASS_WIRELESS_CONTROLLER   = 0xe0
local USB_CLASS_MISC          = 0xef
local USB_CLASS_APP_SPEC      = 0xfe
local USB_CLASS_VENDOR_SPEC       = 0xff

local USB_SUBCLASS_VENDOR_SPEC    = 0xff

local USB_CLASS_VIDEO				= 0x0e

local USB_CLASS_APPLICATION	       	= 0xfe

local VERBLEVEL_DEFAULT = 0	-- 0 gives lspci behaviour; 1, lsusb-0.9

local CTRL_RETRIES	= 2
local CTRL_TIMEOUT	= (5*1000)	-- milliseconds

local HUB_STATUS_BYTELEN	= 3	-- max 3 bytes status = hub + 23 ports

local function printf(format, ...)
	io.write2(string.format(format, ...))
end

local function printf(format, ...)
	trace(string.format(format, ...))
end

local function get_vendor_string(vid)
	return names.vendor[vid] or ''
end

local function le16_to_cpu(x) return x end

local function get_product_string(vid, pid)
	return names.product[vid] and names.product[vid][pid] or ''
end

local function get_class_string(cls)
	return names.class[cls] or ''
end

local function get_subclass_string(cls, subcls)
	return names.subclass[cls] and names.subclass[cls][subcls] or ''
end

local function get_protocol_string(cls, subcls, proto)
	return names.protocol[cls] and names.protocol[cls][subcls] and names.protocol[cls][subcls][proto] or ''
end

local function get_string(dev, id)
	return id >= 1 and dev:get_string_descriptor_ascii(id, 1024) or ''
end

local typeattr = { "Control", "Isochronous", "Bulk", "Interrupt" }
local syncattr = { "None", "Asynchronous", "Adaptive", "Synchronous" }
local usage = { "Data", "Feedback", "Implicit feedback Data", "(reserved)" }
local hb = { "1x", "2x", "3x", "(?\?)" }

local function dump_hub(prefix, p, has_tt)
--	unsigned int l, i, j;
	local wHubChar = p[5] * 2^8 + p[4]

	printf("%sHub Descriptor:\n", prefix)
	printf("%s  bLength             %3u\n", prefix, p[1])
	printf("%s  bDescriptorType     %3u\n", prefix, p[2])
	printf("%s  nNbrPorts           %3u\n", prefix, p[3])
	printf("%s  wHubCharacteristic 0x%04x\n", prefix, wHubChar)
	local tmp = bit.band(wHubChar, 0x03)
--	switch (wHubChar & 0x03) {
	if tmp==tmp0 then
		printf("%s    Ganged power switching\n", prefix);
	elseif tmp==1 then
		printf("%s    Per-port power switching\n", prefix);
	else
		printf("%s    No power switching (usb 1.0)\n", prefix);
	end
	if bit.band(wHubChar, 0x04) > 0 then
		printf("%s    Compound device\n", prefix)
	end
	local tmp = bit.band(bit.rshift(wHubChar, 3), 0x03)
	if tmp==0 then
		printf("%s    Ganged overcurrent protection\n", prefix)
	elseif tmp==1 then
		printf("%s    Per-port overcurrent protection\n", prefix);
	else
		printf("%s    No overcurrent protection\n", prefix);
	end
	if has_tt then
		local l = bit.band(bit.rshift(wHubChar, 5), 0x03)
		printf("%s    TT think time %d FS bits\n", prefix, (l + 1) * 8);
	end
	if bit.band(wHubChar, 2^7) > 0 then
		printf("%s    Port indicators\n", prefix)
	end
	printf("%s  bPwrOn2PwrGood      %3u * 2 milli seconds\n", prefix, p[6])
	printf("%s  bHubContrCurrent    %3u milli Ampere\n", prefix, p[7])
	local l = bit.rshift(p[3], 3) + 1 -- this determines the variable number of bytes following
	if l > HUB_STATUS_BYTELEN then
	 	l = HUB_STATUS_BYTELEN
	end
	printf("%s  DeviceRemovable   ", prefix)
	for i=0,l-1 do
		printf(" 0x%02x", p[8+i])
	end
	printf("\n%s  PortPwrCtrlMask   ", prefix)
 	for j=0,l-1 do
 		printf(" 0x%02x", p[8+l+j]);
	end
 	printf("\n");
end

local function dump_endpoint(dev, interface, endpoint)
	local wmax = le16_to_cpu(endpoint.wMaxPacketSize)

	printf([[
      Endpoint Descriptor:
        bLength             %5u
        bDescriptorType     %5u
        bEndpointAddress     0x%02x  EP %u %s
        bmAttributes        %5u
          Transfer Type            %s
          Synch Type               %s
          Usage Type               %s
        wMaxPacketSize     0x%04x  %s %d bytes
        bInterval           %5u
]],
	       endpoint.bLength, endpoint.bDescriptorType, endpoint.bEndpointAddress, endpoint.bEndpointAddress % 16,
	       bit.band(endpoint.bEndpointAddress, 0x80)~=0 and "IN" or "OUT", endpoint.bmAttributes,
	       typeattr[endpoint.bmAttributes % 4 + 1], syncattr[math.floor(endpoint.bmAttributes / 2^2) % 4 + 1],
	       usage[math.floor(endpoint.bmAttributes / 2^4) % 4 + 1],
	       wmax, hb[math.floor(wmax / 2^11) % 4 + 1], wmax % 0x800,
	       endpoint.bInterval);
	-- only for audio endpoints
	if endpoint.bLength == 9 then
		printf([[
        bRefresh            %5u
        bSynchAddress       %5u
]],
		       endpoint.bRefresh, endpoint.bSynchAddress)
	end

	-- avoid re-ordering or hiding descriptors for display
	if endpoint.extra then
		local size = #endpoint.extra
		local buf = endpoint.extra
		while size >= 2 do
			local ds = string.byte(buf, 1)
			if ds < 2 then
				dump_junk(buf, "        ", size)
				break
			end
			local dt = string.byte(buf, 2)
			if dt==assert(USB_DT_CS_ENDPOINT) then
				if interface.bInterfaceClass == 1 and interface.bInterfaceSubClass == 2 then
					dump_audiostreaming_endpoint(buf)
				elseif interface.bInterfaceClass == 1 and interface.bInterfaceSubClass == 3 then
					dump_midistreaming_endpoint(buf)
				end
			elseif dt==assert(USB_DT_CS_INTERFACE) then
				-- MISPLACED DESCRIPTOR ... less indent
				local ic = interface.bInterfaceClass
				if interface.bInterfaceClass==assert(USB_CLASS_COMM)
				or interface.bInterfaceClass==assert(USB_CLASS_DATA) then -- comm data
					dump_comm_descriptor(dev, buf,
						"      ")
				else
					printf("        INTERFACE CLASS: ")
					dump_bytes(buf, ds)
				end
			elseif dt==assert(USB_DT_OTG) then
				-- handled separately
			elseif dt==assert(USB_DT_INTERFACE_ASSOCIATION) then
				dump_association(dev, buf)
			else
				-- often a misplaced class descriptor
				printf("        ** UNRECOGNIZED: ")
				dump_bytes(buf, ds)
			end
			size = size - ds
			buf = buf:sub(ds+1)
		end
	end
end
local function dump_bytes(buf, len)
	for i=1,len do
		printf(" %02x", string.byte(buf, i))
	end
	printf("\n")
end

local function dump_junk(buf, indent, len)
	local ds = string.byte(buf, 1)
	if ds <= len then
		return
	end
	printf("%sjunk at descriptor end:", indent)
	for i=len+1,ds do
		printf(" %02x", string.byte(buf, i))
	end
	printf("\n")
end

local function dump_hid_device(dev, interface, buf)
	local ds = string.byte(buf, 1)
	local dt = string.byte(buf, 2)
	if dt ~= usb.DT_HID then
		printf("      Warning: Invalid descriptor\n")
	elseif ds < 6+3*string.byte(buf, 6) then
		printf("      Warning: Descriptor too short\n");
	end
	printf([[
        HID Device Descriptor:
          bLength             %5u
          bDescriptorType     %5u
          bcdHID              %2x.%02x
          bCountryCode        %5u %s
          bNumDescriptors     %5u
]],
	       string.byte(buf, 1), string.byte(buf, 2), string.byte(buf, 4), string.byte(buf, 3), string.byte(buf, 5),
	       names.countrycode[string.byte(buf, 5)] or "Unknown", string.byte(buf, 6))
	local bNumDescriptors = string.byte(buf, 6)
	for i=0,bNumDescriptors-1 do
		local bDescriptorType = string.byte(buf, 7+3*i)
		printf([[
          bDescriptorType     %5u %s
          wDescriptorLength   %5u
]],
		       bDescriptorType, names.hid[bDescriptorType],
		       string.byte(buf, 8+3*i) + string.byte(buf, 9+3*i) * 2^8)
	end
	dump_junk(buf, "        ", 6+3*string.byte(buf, 6))
	if not do_report_desc then
		return
	end

	if not dev then
		printf([[
         Report Descriptors: 
           ** UNAVAILABLE **
]])
		return
	end

	for i=0,string.byte(buf, 6)-1 do
		-- we are just interested in report descriptors
		if string.byte(buf, 7+3*i) == usb.DT_REPORT then

		local len = string.byte(buf, 8+3*i) + string.byte(buf, 9+3*i) * 2^8
	--	if len > sizeof(dbuf) then
	--		printf("report descriptor too long\n")
	--		continue
	--	end
		if dev:claim_interface(interface.bInterfaceNumber) then
			local retries = 3
			local dbuf,msg
			while not dbuf and retries > 0 do
				dbuf,msg = dev:control_transfer(
					bit.bor(
						usb.ENDPOINT_IN,
						usb.REQUEST_TYPE_STANDARD,
						usb.RECIPIENT_INTERFACE),
					usb.REQUEST_GET_DESCRIPTOR,
					usb.DT_REPORT * 2^8,
					interface.bInterfaceNumber,
					len,
					CTRL_TIMEOUT)
				retries = retries - 1
			 end

			if dbuf then
				if #dbuf < len then
					printf("          Warning: incomplete report descriptor\n")
				end
				if lua_report then
					dump_report_desc_lua(dbuf)
				else
					dump_report_desc(dbuf, #dbuf)
				end
			end
			dev:release_interface(interface.bInterfaceNumber)
		else
			-- recent Linuxes require claim() for RECIP_INTERFACE,
			-- so "rmmod hid" will often make these available.
			printf([[
         Report Descriptors: 
           ** UNAVAILABLE **
]]);
		end

		end
	end
end

local function dump_altsetting(dev, interface)
	local cls = get_class_string(interface.bInterfaceClass)
	local subcls = get_subclass_string(interface.bInterfaceClass, interface.bInterfaceSubClass)
	local proto = get_protocol_string(interface.bInterfaceClass, interface.bInterfaceSubClass, interface.bInterfaceProtocol)
	local ifstr = get_string(dev, interface.iInterface)
	printf([[
    Interface Descriptor:
      bLength             %5u
      bDescriptorType     %5u
      bInterfaceNumber    %5u
      bAlternateSetting   %5u
      bNumEndpoints       %5u
      bInterfaceClass     %5u %s
      bInterfaceSubClass  %5u %s
      bInterfaceProtocol  %5u %s
      iInterface          %5u %s
]],
	       interface.bLength, interface.bDescriptorType, interface.bInterfaceNumber,
	       interface.bAlternateSetting, interface.bNumEndpoints, interface.bInterfaceClass, cls,
	       interface.bInterfaceSubClass, subcls, interface.bInterfaceProtocol, proto,
	       interface.iInterface, ifstr)

	-- avoid re-ordering or hiding descriptors for display
	if interface.extra then
		local size = #interface.extra
		local buf = interface.extra
		while size >= 2 do
			local ds = string.byte(buf, 1)
			if ds < 2 then
				dump_junk(buf, "      ", size)
				break
			end

			local dt = string.byte(buf, 2)

			local function dump()
				printf("      ** UNRECOGNIZED: ")
				dump_bytes(buf, ds)
			end

			-- This is the polite way to provide class specific
			-- descriptors: explicitly tagged, using common class
			-- spec conventions.
			if dt==assert(USB_DT_CS_DEVICE)
			or dt==assert(USB_DT_CS_INTERFACE) then
				if interface.bInterfaceClass==assert(USB_CLASS_AUDIO) then
					if interface.bInterfaceSubClass==1 then
						dump_audiocontrol_interface(dev, buf)
					elseif interface.bInterfaceSubClass==2 then
						dump_audiostreaming_interface(buf)
					elseif interface.bInterfaceSubClass==3 then
						dump_midistreaming_interface(dev, buf)
					else
						dump()
					end
				elseif interface.bInterfaceClass==assert(USB_CLASS_COMM) then
					dump_comm_descriptor(dev, buf,
						"      ")
				elseif interface.bInterfaceClass==assert(USB_CLASS_VIDEO) then
					if interface.bInterfaceSubClass==1 then
						dump_videocontrol_interface(dev, buf)
					elseif interface.bInterfaceSubClass==2 then
						dump_videostreaming_interface(buf)
					else
						dump()
					end
				elseif interface.bInterfaceClass==assert(USB_CLASS_APPLICATION) then
					if interface.bInterfaceSubClass==1 then
						--dump_dfu_interface(buf)
					else
						dump()
					end
				elseif interface.bInterfaceClass==assert(USB_CLASS_HID) then
					dump_hid_device(dev, interface, buf)
				else
					dump()
				end

			-- This is the ugly way:  implicitly tagged,
			-- each class could redefine the type IDs.
			else
			--	switch (interface->bInterfaceClass) {
				if interface.bInterfaceClass==assert(USB_CLASS_HID) then
					dump_hid_device(dev, interface, buf)
				elseif interface.bInterfaceClass==0x0b then -- chip/smartcard
					dump_ccid_device(buf)
				elseif interface.bInterfaceClass==0xe0 then -- wireless
					if interface.bInterfaceSubClass==1 then
						if interface.bInterfaceProtocol==2 then
							dump_rc_interface(buf)
						else
							dump()
						end
					elseif interface.bInterfaceSubClass==2 then
						dump_wire_adapter(buf)
					else
						dump()
					end
				else
					-- ... not everything is class-specific
					local dt = string.byte(buf, 2)
					if dt==assert(USB_DT_OTG) then
						-- handled separately
					elseif dt==assert(USB_DT_INTERFACE_ASSOCIATION) then
						dump_association(dev, buf);
					else
						-- often a misplaced class descriptor
						dump()
					end
				end
			end
			size = size - ds
			buf = buf:sub(ds+1)
		end
	end

	for i=1,interface.bNumEndpoints do
		dump_endpoint(dev, interface, interface.endpoint[i])
	end
end

local function dump_interface(dev, interface)
	for i=1,interface.num_altsetting do
		dump_altsetting(dev, interface.altsetting[i])
	end
end



local function dump_config(dev, config)
	local cfg = get_string(dev, config.iConfiguration)
	printf([[
  Configuration Descriptor:
    bLength             %5u
    bDescriptorType     %5u
    wTotalLength        %5u
    bNumInterfaces      %5u
    bConfigurationValue %5u
    iConfiguration      %5u %s
    bmAttributes         0x%02x
]],
	       config.bLength, config.bDescriptorType,
	       le16_to_cpu(config.wTotalLength),
	       config.bNumInterfaces, config.bConfigurationValue,
	       config.iConfiguration,
	       cfg, config.bmAttributes)
	if bit.band(config.bmAttributes, 0x80)==0 then
		printf("      (Missing must-be-set bit!)\n")
	end
	if bit.band(config.bmAttributes, 0x40)~=0 then
		printf("      Self Powered\n")
	else
		printf("      (Bus Powered)\n")
	end
	if bit.band(config.bmAttributes, 0x20)~=0 then
		printf("      Remote Wakeup\n");
	end
	if bit.band(config.bmAttributes, 0x10)~=0 then
		printf("      Battery Powered\n")
	end
	printf("    MaxPower            %5umA\n", config.MaxPower * 2)

	-- avoid re-ordering or hiding descriptors for display
	if config.extra then
		local size = #config.extra
		local buf = string.byte(config.extra, 1, size)

		while size >= 2 do
			local ds = string.byte(buf, 1)
			if ds < 2 then
				dump_junk(buf, "        ", size)
				break
			end
			local dt = string.byte(buf, 2)
			if dt==assert(USB_DT_OTG) then
				-- handled separately
			elseif dt==assert(USB_DT_INTERFACE_ASSOCIATION) then
				dump_association(dev, buf)
			elseif dt==assert(USB_DT_SECURITY) then
				dump_security(buf)
			elseif dt==assert(USB_DT_ENCRYPTION_TYPE) then
				dump_encryption_type(buf)
			else
				-- often a misplaced class descriptor
				printf("    ** UNRECOGNIZED: ")
				dump_bytes(buf, ds)
			end
			size = size - ds
			buf = buf:sub(ds+1)
		end
	end
	for i=1,config.bNumInterfaces do
		dump_interface(dev, config.interface[i])
	end
end

local function dump_device(dev, descriptor)
	local vendor = get_vendor_string(descriptor.idVendor)
	local product = get_product_string(descriptor.idVendor, descriptor.idProduct)
	local cls = get_class_string(descriptor.bDeviceClass)
	local subcls = get_subclass_string(descriptor.bDeviceClass, descriptor.bDeviceSubClass)
	local proto = get_protocol_string(descriptor.bDeviceClass, descriptor.bDeviceSubClass, descriptor.bDeviceProtocol)
	local mfg = get_string(dev, descriptor.iManufacturer)
	local prod = get_string(dev, descriptor.iProduct)
	local serial = get_string(dev, descriptor.iSerialNumber)
	printf([[
Device Descriptor:
  bLength             %5u
  bDescriptorType     %5u
  bcdUSB              %2x.%02x
  bDeviceClass        %5u %s
  bDeviceSubClass     %5u %s
  bDeviceProtocol     %5u %s
  bMaxPacketSize0     %5u
  idVendor           0x%04x %s
  idProduct          0x%04x %s
  bcdDevice           %2x.%02x
  iManufacturer       %5u %s
  iProduct            %5u %s
  iSerial             %5u %s
  bNumConfigurations  %5u
]],
	       descriptor.bLength, descriptor.bDescriptorType,
	       math.floor(descriptor.bcdUSB / 256), descriptor.bcdUSB % 256,
	       descriptor.bDeviceClass, cls,
	       descriptor.bDeviceSubClass, subcls,
	       descriptor.bDeviceProtocol, proto,
	       descriptor.bMaxPacketSize0,
	       descriptor.idVendor, vendor, descriptor.idProduct, product,
	       math.floor(descriptor.bcdDevice / 256), descriptor.bcdDevice % 256,
	       descriptor.iManufacturer, mfg,
	       descriptor.iProduct, prod,
	       descriptor.iSerialNumber, serial,
	       descriptor.bNumConfigurations)
end
local function do_hub(fd, has_tt)
	local bufsize = 7 --[[ base descriptor ]]
			+ 2 --[[ bitmasks ]] * assert(HUB_STATUS_BYTELEN)
	local buf

	buf = assert(fd:control_transfer(
			bit.bor(
				usb.ENDPOINT_IN,
				usb.REQUEST_TYPE_CLASS,
				usb.RECIPIENT_DEVICE),
			usb.REQUEST_GET_DESCRIPTOR,
			usb.DT_HUB * 2^8, 0,
			bufsize, CTRL_TIMEOUT))
	if #buf < 9 --[[ at least one port's bitmasks ]] then
		error()
		return
	end
	buf = {string.byte(buf, 1, #buf)}
	dump_hub("", buf, has_tt)

	printf(" Hub Port Status:\n")
	for i=0,buf[3]-1 do

		local stat = assert(fd:control_transfer(
				bit.bor(
					usb.ENDPOINT_IN,
					usb.REQUEST_TYPE_CLASS,
					usb.RECIPIENT_OTHER),
				usb.REQUEST_GET_STATUS,
				0, i + 1,
				4,
				CTRL_TIMEOUT))
		stat = {string.byte(stat, 1, #stat)}

		printf("   Port %d: %02x%02x.%02x%02x", i + 1,
			stat[4], stat [3],
			stat[2], stat [1])
		-- CAPS are used to highlight "transient" states
		printf("%s%s%s%s%s",
			bit.band(stat[3], 0x10) > 0 and " C_RESET" or "",
			bit.band(stat[3], 0x08) > 0 and " C_OC" or "",
			bit.band(stat[3], 0x04) > 0 and " C_SUSPEND" or "",
			bit.band(stat[3], 0x02) > 0 and " C_ENABLE" or "",
			bit.band(stat[3], 0x01) > 0 and " C_CONNECT" or "")
		printf("%s%s%s%s%s%s%s%s%s%s\n",
			bit.band(stat[2], 0x10) > 0 and " indicator" or "",
			bit.band(stat[2], 0x08) > 0 and " test" or "",
			bit.band(stat[2], 0x04) > 0 and " highspeed" or "",
			bit.band(stat[2], 0x02) > 0 and " lowspeed" or "",
			bit.band(stat[2], 0x01) > 0 and " power" or "",
			bit.band(stat[1], 0x10) > 0 and " RESET" or "",
			bit.band(stat[1], 0x08) > 0 and " oc" or "",
			bit.band(stat[1], 0x04) > 0 and " suspend" or "",
			bit.band(stat[1], 0x02) > 0 and " enable" or "",
			bit.band(stat[1], 0x01) > 0 and " connect" or "")
	end
end
local function do_debug(fd)
	local bufsize = 4

	local buf,msg = fd:control_transfer(
			bit.bor(
				usb.ENDPOINT_IN,
				usb.REQUEST_TYPE_STANDARD,
				usb.RECIPIENT_DEVICE),
			usb.REQUEST_GET_DESCRIPTOR,
			assert(USB_DT_DEBUG) * 2^8, 0,
			bufsize, CTRL_TIMEOUT)
	if not buf and msg~='pipe' then
		print("can't get debug descriptor")
	end
	if not buf then return end
	buf = {string.byte(buf, 1, #buf)}

	-- some high speed devices are also "USB2 debug devices", meaning
	-- you can use them with some EHCI implementations as another kind
	-- of system debug channel:  like JTAG, RS232, or a console.
	if #buf ~= bufsize
			or buf[1] ~= #buf
			or buf[12] ~= assert(USB_DT_DEBUG) then
		return
	end

	printf([[
Debug descriptor:\n"
  bLength              %4u
  bDescriptorType      %4u
  bDebugInEndpoint     0x%02x
  bDebugOutEndpoint    0x%02x
]],
	       buf[1], buf[2],
	       buf[3], buf[4])
end

local function do_dualspeed(fd)
--	unsigned char buf [10];
--	char cls[128], subcls[128], proto[128];
--	int ret;
	local bufsize,buf = 10

	local buf,msg = fd:control_transfer(
			bit.bor(
				usb.ENDPOINT_IN,
				usb.REQUEST_TYPE_STANDARD,
				usb.RECIPIENT_DEVICE),
			usb.REQUEST_GET_DESCRIPTOR,
			assert(USB_DT_DEVICE_QUALIFIER) * 2^8, 0,
			bufsize, CTRL_TIMEOUT)
	if not buf and msg~='pipe' then
		print("can't get device qualifier")
	end
	if not buf then return end
	buf = {string.byte(buf, 1, #buf)}

	-- all dual-speed devices have a qualifier
	if #buf ~= bufsize
			or buf[1] ~= #buf
			or buf[2] ~= assert(USB_DT_DEVICE_QUALIFIER) then
		return
	end

	local cls = get_class_string(buf[5])
	local subcls = get_subclass_string(buf[5], buf[6])
	local proto = get_protocol_string(buf[5], buf[6], buf[7])
	printf([[
Device Qualifier (for other device speed):
  bLength             %5u
  bDescriptorType     %5u
  bcdUSB              %2x.%02x
  bDeviceClass        %5u %s
  bDeviceSubClass     %5u %s
  bDeviceProtocol     %5u %s
  bMaxPacketSize0     %5u
  bNumConfigurations  %5u
]],
	       buf[1], buf[2],
	       buf[4], buf[3],
	       buf[5], cls,
	       buf[6], subcls,
	       buf[7], proto,
	       buf[8], buf[9])

	-- FIXME also show the OTHER_SPEED_CONFIG descriptors
end

local function dump_device_status(fd, otg, wireless)
	local status
--	int ret;

	status = assert(fd:control_transfer(
			bit.bor(
				usb.ENDPOINT_IN,
				usb.REQUEST_TYPE_STANDARD,
				usb.RECIPIENT_DEVICE),
			usb.REQUEST_GET_STATUS,
			0, 0,
			2,
			CTRL_TIMEOUT))
--	if (ret < 0) {
--		fprintf(stderr,
--			"cannot read device status, %s (%d)\n",
--			strerror(errno), errno);
--		return;
--	}
	status = {string.byte(status, 1, #status)}

	printf("Device Status:     0x%02x%02x\n",
			status[2], status[1])
	if bit.band(status[1], 2^0) > 0 then
		printf("  Self Powered\n")
	else
		printf("  (Bus Powered)\n")
	end
	if bit.band(status[1], 2^1) > 0 then
		printf("  Remote Wakeup Enabled\n")
	end
	if bit.band(status[1], 2^2) > 0 then
		-- for high speed devices
		if not wireless then
			printf("  Test Mode\n")
		-- for devices with Wireless USB support
		else
			printf("  Battery Powered\n")
		end
	end
	-- if both HOST and DEVICE support OTG
	if otg then
		if bit.band(status[1], 2^3) > 0 then
			printf("  HNP Enabled\n")
		end
		if bit.band(status[1], 2^4) > 0 then
			printf("  HNP Capable\n")
		end
		if bit.band(status[1], 2^5) > 0 then
			printf("  ALT port is HNP Capable\n")
		end
	end
	-- for high speed devices with debug descriptors
	if bit.band(status[1], 2^6) > 0 then
		printf("  Debug Mode\n")
	end

	if not wireless then
		return
	end

	-- Wireless USB exposes FIVE different types of device status,
	-- accessed by distinct wIndex values.
	status = assert(fd:control_transfer(
			bit.bor(
				usb.ENDPOINT_IN,
				usb.REQUEST_TYPE_STANDARD,
				usb.RECIPIENT_DEVICE),
			usb.REQUEST_GET_STATUS,
			0, 1 --[[ wireless status ]],
			1,
			CTRL_TIMEOUT))
	status = {string.byte(status, 1, #status)}
	printf("Wireless Status:     0x%02x\n", status[1])
	if bit.band(status[1], 2^0) > 0 then
		printf("  TX Drp IE\n")
	end
	if bit.band(status[1], 2^1) > 0 then
		printf("  Transmit Packet\n")
	end
	if bit.band(status[1], 2^2) > 0 then
		printf("  Count Packets\n")
	end
	if bit.band(status[1], 2^3) > 0 then
		printf("  Capture Packet\n")
	end

	status = assert(fd:control_transfer(
			bit.bor(
				usb.ENDPOINT_IN,
				usb.REQUEST_TYPE_STANDARD,
				usb.RECIPIENT_DEVICE),
			usb.REQUEST_GET_STATUS,
			0, 2 --[[ Channel Info ]],
			1,
			CTRL_TIMEOUT))
	status = {string.byte(status, 1, #status)}
	printf("Channel Info:        0x%02x\n", status[1])

	-- 3=Received data: many bytes, for count packets or capture packet

	status = assert(fd:control_transfer(
			bit.bor(
				usb.ENDPOINT_IN,
				usb.REQUEST_TYPE_STANDARD,
				usb.RECIPIENT_DEVICE),
			usb.REQUEST_GET_STATUS,
			0, 3 --[[ MAS Availability ]],
			8,
			CTRL_TIMEOUT))

	status = {string.byte(status, 1, #status)}
	printf("MAS Availability:    ")
	dump_bytes(status, 8)

	status = assert(fd:control_transfer(
			bit.bor(
				usb.ENDPOINT_IN,
				usb.REQUEST_TYPE_STANDARD,
				usb.RECIPIENT_DEVICE),
			usb.REQUEST_GET_STATUS,
			0, 5 --[[ Current Transmit Power ]],
			2,
			CTRL_TIMEOUT))

	status = {string.byte(status, 1, #status)}
	printf("Transmit Power:\n");
	printf(" TxNotification:     0x%02x\n", status[1]);
	printf(" TxBeacon:     :     0x%02x\n", status[2]);
end

local function dumpdev(dev)
	local otg,wireless

	local handle = dev:open()
	if not handle then
		printf("Couldn't open device, some information will be missing\n")
		return
	end

	local descriptor = assert(dev:get_device_descriptor())
	
	dump_device(handle, descriptor)
	if descriptor.bcdUSB >= 0x0250 then
		wireless = do_wireless(handle)
	end
	if descriptor.bNumConfigurations > 0 then
	--	local config = dev:get_config_descriptor(0)
	--	otg = do_otg(config) or otg
		for i=0,descriptor.bNumConfigurations-1 do
			local config_desc = assert(dev:get_config_descriptor(i))
			dump_config(handle, config_desc)
		end
	end
	
	if descriptor.bDeviceClass == usb.CLASS_HUB then
		do_hub(handle, descriptor.bDeviceProtocol)
	end
	if descriptor.bcdUSB >= 0x0200 then
		do_dualspeed(handle)
		do_debug(handle)
	end
	dump_device_status(handle, otg, wireless)
	handle:close()
end

--[[
local  dfu_transfer_out( handle,  request,  value,  data,  length )
{
      handle:control_transfer( LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
												request, value,  handle.interface,  data,  length,  DFU_TIMEOUT );
}

local dfu_abort( handle )
{
    local result;
	
    result = dfu_transfer_out( handle, DFU_ABORT, 0, NULL, 0 );

    dfu_msg_response_output( __FUNCTION__, result );

    return result;
}


/*
 *  DFU_ABORT Request (DFU Spec 1.0, Section 6.1.4)
 *
 *  device    - the dfu device to commmunicate with
 *
 *  returns 0 or < 0 on an error
 */
int32_t dfu_abort( dfu_device_t *device )
{
    int32_t result;

    TRACE( "%s( %p )\n", __FUNCTION__, device );

    if( (NULL == device) || (NULL == device->handle) ) {
        DEBUG( "Invalid parameter\n" );
        return -1;
    }

    result = dfu_transfer_out( device, DFU_ABORT, 0, NULL, 0 );

    dfu_msg_response_output( __FUNCTION__, result );

    return result;
}


local  dfu_make_idle( dfu_device_t *device,
                              const dfu_bool initial_abort )
{
    dfu_status_t status;
    int32_t retries = 4;

    if( true == initial_abort ) {
        dfu_abort( device );
    }

    while( 0 < retries ) {
        if( 0 != dfu_get_status(device, &status) ) {
            dfu_clear_status( device );
            continue;
        }

        DEBUG( "State: %s (%d)\n", dfu_state_to_string(status.bState), status.bState );

        switch( status.bState ) {
            case STATE_DFU_IDLE:
                if( DFU_STATUS_OK == status.bStatus ) {
                    return 0;
                }

                /* We need the device to have the DFU_STATUS_OK status. */
                dfu_clear_status( device );
                break;

            case STATE_DFU_DOWNLOAD_SYNC:   /* abort -> idle */
            case STATE_DFU_DOWNLOAD_IDLE:   /* abort -> idle */
            case STATE_DFU_MANIFEST_SYNC:   /* abort -> idle */
            case STATE_DFU_UPLOAD_IDLE:     /* abort -> idle */
            case STATE_DFU_DOWNLOAD_BUSY:   /* abort -> error */
            case STATE_DFU_MANIFEST:        /* abort -> error */
                dfu_abort( device );
                break;

            case STATE_DFU_ERROR:
                dfu_clear_status( device );
                break;

            case STATE_APP_IDLE:
                dfu_detach( device, DFU_DETACH_TIMEOUT );
                break;

            case STATE_APP_DETACH:
            case STATE_DFU_MANIFEST_WAIT_RESET:
                DEBUG( "Resetting the device\n" );
#ifdef HAVE_LIBUSB_1_0
                libusb_reset_device( device->handle );
#else
                usb_reset( device->handle );
#endif
                return 1;
        }

        retries--;
    }

    DEBUG( "Not able to transition the device into the dfuIDLE state.\n" );
    return -2;
}

--]]

local function list_devices(busnum, devnum, vendorid, productid)
	local context = assert(usb.init())
	--context:set_debug(3)
	local devices = assert(context:get_device_list())
	for _,device in ipairs(devices) do
		local descriptor = assert(device:get_device_descriptor())
		if (not vendorid or vendorid==descriptor.idVendor)
			and (not productid or productid==descriptor.idProduct) then
			local vendor = get_vendor_string(descriptor.idVendor)
			local product = get_product_string(descriptor.idVendor, descriptor.idProduct)
			if verblevel > 0 then
				printf("\n")
			end
			printf("Bus %03d Device %03d: ID %04x:%04x %s %s\n",
					device:get_bus_number(),
					device:get_device_address(),
					descriptor.idVendor,
					descriptor.idProduct,
					vendor, product);
			if verblevel > 0 then
				dumpdev(device)
			end
	
			local handle = device:open()
			if not handle then
				printf("Couldn't open device, some information will be missing\n")
				return
			end
			printf("device opened\n")
			handle:set_configuration(1)
			if handle:claim_interface(0) then
				printf("Success\n ")
				
				--[[
for i=1,100 do
	local result,msg = handle:interrupt_transfer(0x81, 8, 0)
	if result then
		local bytes = {string.byte(result, 1, #result)}
		for _,byte in ipairs{string.byte(result, 1, #result)} do
			io.write(string.format("%02x", byte))
		end
		io.write("\n")
	end
end
--]]


				--[[
			
			                            switch( dfu_make_idle(dfu_device, initial_abort) )
                            {
                                case 0:
                                    libusb_free_device_list( list, 1 );
                                    return device;

                                case 1:
                                    retries--;
                                    libusb_free_device_list( list, 1 );
                                    goto retry;
                            }

                            DEBUG( "Failed to put the device in dfuIDLE mode.\n" );
                            libusb_release_interface( dfu_device->handle, dfu_device->interface );
		
						--]]
					print("releasing interface\n")
					print(handle:release_interface(0))
			end
		end
	end
end

--list_devices(busnum, devnum, vendorid, productid)

list_devices(busnum, devnum, 0x03eb, 0x2ff8)




