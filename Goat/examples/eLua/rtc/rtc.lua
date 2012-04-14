-- Check platform
if pd.board() ~= 'MIZAR32' then
  print( pd.board() .. " not supported by this example" )
  return
end

--mizar32.rtc.set( { hour=10, min=54 } )

print("hour: " .. mizar32.rtc.get().hour .. " minute: " .. mizar32.rtc.get().min)