-- Check platform
if pd.board() ~= 'MIZAR32' then
  print( pd.board() .. " not supported by this example" )
  return
end

uart.setup( 0, 115200, 8, uart.PAR_NONE, uart.STOP_1 )
pio.pin.setdir(pio.INPUT, pio.PX_16)

print ("Press the \"User\" button to exit..." )

while (pio.pin.getval(pio.PX_16) == 1) do
	uart.write(0,uart.read(0xB0,20,500 ))
	uart.write(0xB0,uart.read(0,20,500 ))
end

print ("done." )
