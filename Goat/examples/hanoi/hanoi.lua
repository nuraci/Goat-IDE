-- Towers of Hanoi
local term = term

--for k, v in pairs(term) do print(k,v)end
--do return end

-------------------------------------------------------------------------------
-- Data structures

local schar = '#' 
local bchar =  '#'


local gxoff, lxoff = 4, 3
local gyoff = 28
local tower_height = 12
local topline = 14
local totdisks = 2

-------------------------------------------------------------------------------
-- Disk class

local disk = {}
disk.__index = disk

disk.new = function( size, x, y )
  local self = {}
  self.size = size
  self.x = x
  self.y = y
  setmetatable( self, disk )
  return self
end

disk.draw = function( self, clear )  
  local c = clear and ' ' or schar
  local ddata = string.rep( c, self.size * 2 + 2 )
  term.print("\27[32;0m")
  term.print( self.x, self.y, ddata )
  term.print("\27[0;0m")
end

disk.move = function( self, dx, dy )
  self:draw( true )
  self.x = self.x + dx
  self.y = self.y + dy
  self:draw()
end

-------------------------------------------------------------------------------
-- Towers class

local tower = {}
tower.__index = tower

tower.new = function( id, ndisks )
  local self = {}
  self.id = id
  self.disks = {}
  local topsize = ndisks > 0 and 1 or 0
  self.x = gxoff + lxoff + 24 * id
  local y = gyoff - ( ndisks - 1 )
  for i = topsize, topsize + ndisks - 1 do
    self.disks[ #self.disks + 1 ] = disk.new( i, self.x + 8 - i, y )
    y = y + 1
  end
  setmetatable( self, tower )
  return self
end

tower.draw = function( self, clear )
  -- Draw center line
  term.print("\27[34;0m")
  for y = gyoff - #self.disks, gyoff - tower_height - 1, -1 do
    term.print( self.x + 8, y, bchar )
    term.print( self.x + 9, y, bchar )
  end
  term.print("\27[0;0m")
  for i = 1, #self.disks do
    self.disks[ i ]:draw()
  end
end

tower._topsize = function( self )
  return #self.disks == 0 and 0 or self.disks[ 1 ].size
end

tower._numdisks = function( self )
  return #self.disks
end

tower.canmove = function( self, d )
  return #self.disks == 0 and 0 or d.size < self:_topsize()
end

tower.add_disk = function( self, d )
  if self:canmove( d ) then
    for i = #self.disks, 1, -1 do
      self.disks[ i + 1 ] = self.disks[ i ]
    end
    self.disks[ 1 ] = d
  end
end

tower.remove_disk = function( self )
  if self.ndisks == 0 then return nil end
  local d = self.disks[ 1 ]
  for i = 2, #self.disks do
    self.disks[ i - 1 ] = self.disks[ i ]
  end
  self.disks[ #self.disks ] = nil
  return d
end

tower.diskpos = function( self, d )
  local x = self.x + 8 - d.size
  local y = gyoff - ( #self.disks == 0 and 0 or #self.disks )
  return x, y
end

-------------------------------------------------------------------------------
-- User interface
local function center( text, y )
  local x = ( term.getcols() - #text ) / 2
  term.print( x, y, text )
end

local function show_menu()
  term.print("\27[33;0m")

  center( "C: New computer game", 7 )
  center( "P: New player game", 8 )
  center( "Q: Quit game", 9 )
  term.print("\27[0;0m")
end

local function clear_menu()
  term.print("\27[0;0m")
  for i = 7, 11 do
    term.moveto( 0, i )
    term.clreol()
  end
end

local function get_menu_option()
  local sc
  while true do
    local c = term.getchar()
    sc = string.char( c ):upper()
    if sc == 'Q' or sc == 'P' or sc =='C' then break end
  end
  return sc
end

local function show_logo()
  local sx = 27
  term.print("\27[31;0m")

  term.print( sx, 1, "#   #   #   #   #  ###  #" )
  term.print( sx, 2, "#   #  # #  ##  # #   # #" )
  term.print( sx, 3, "##### ##### # # # #   # #" )
  term.print( sx, 4, "#   # #   # #  ## #   # #" )
  term.print( sx, 5, "#   # #   # #   #  ###  #" )
  term.print("\27[00;0m")
end

-------------------------------------------------------------------------------

local function totower( from, to )
  -- Step 1: go up
  local d = from:remove_disk()
  local y = d.y
  while y >= topline do
    d:move( 0, -1 )
    from:draw()
    tmr.delay( 0, 50000 )
    if term.getchar( term.NOWAIT ) ~= -1 then return true end
    y = y - 1
  end
  to:add_disk( d )
  -- Step 1: move left/right
  local dir = to.id > from.id and 2 or -2
  local nx, ny = to:diskpos( d )
  local x = d.x
  while x ~= nx do
    d:move( dir, 0 )
    d:draw()
    tmr.delay( 0, 50000 )
    if term.getchar( term.NOWAIT ) ~= -1 then return true end
    x = x + dir
  end
  -- Step 3: go down
  local y = d.y
  repeat
    d:move( 0, 1 )
    to:draw()
    tmr.delay( 0, 50000 )
    if term.getchar( term.NOWAIT ) ~= -1 then return true end
    y = y + 1
  until y > ny
end

local function canmove( t1, t2 )
  if t1:_numdisks() == 0 then return false end
  local d = t1.disks[ 1 ]
  return t2:canmove( d )
end

local function clear_towers()
  for y = gyoff, gyoff - tower_height - 3, -1 do
    term.moveto( 0, y )
    term.clreol()
  end
end

local function create_towers( ndisks )
  ndisks = ndisks or 0
  local towers = {}
  for i = 1, 3 do
    towers[ i ] = tower.new( i - 1, i == 1 and ndisks or 0 )
  end
  return towers
end

local function draw_towers()
  for i = 1, 3 do
    towers[ i ]:draw()
  end  
end

term.clrscr()
show_logo()
while true do
  clear_towers()
  towers = create_towers()
  draw_towers()
  clear_menu()
  show_menu()
  local c = get_menu_option()
  if c == 'Q' then
    term.clrscr()
    do return end
  elseif c == 'C' then
    clear_menu()
	term.print("\27[34;0m")

    center( "Number of disks: ", 8 )
    while true do
      totdisks = string.char( term.getchar() )
      totdisks = tonumber( totdisks )
      if type( totdisks ) == "number" and totdisks > 1 and totdisks < 9 then
        break
      end
    end
	term.print("\27[33;0m")
    term.print( tostring( totdisks ) )
    center( 'Press any key to stop', 10 )    
    towers = create_towers( totdisks )  
    draw_towers()
    local A, B, C = towers[ 1 ], towers[ 2 ], towers[ 3 ]
    local res
    local t1 = totdisks % 2 == 0 and B or C
    local t2 = totdisks % 2 == 0 and C or B
    while true do
      -- A to t1
      if canmove( A, t1 ) then res = totower( A, t1 ) else res = totower( t1, A ) end
      if res then break end
      if C:_numdisks() == totdisks then break end
      -- A to t2
      if canmove( A, t2 ) then res = totower( A, t2 ) else res = totower( t2, A ) end
      if res then break end
      if C:_numdisks() == totdisks then break end
      -- B to C
      if canmove( B, C ) then res = totower( B, C ) else res = totower( C, B ) end
      if res then break end
      if C:_numdisks() == totdisks then break end
    end
    if not res then term.getchar() end
  elseif c == 'P' then  
    clear_menu()
	term.print("\27[36;0m")
    center( "Number of disks: ", 7 )
    while true do
      totdisks = string.char( term.getchar() )
      totdisks = tonumber( totdisks )
      if type( totdisks ) == "number" and totdisks > 1 and totdisks < 9 then
        break
      end
    end
	term.print("\27[33;0m")
    term.print( tostring( totdisks ) )    
    center( "Press 1, 2 or 3 to select tower", 8 )
    center( "Press 'Q' to exit", 9 )
    towers = create_towers( totdisks )  
    draw_towers()
    local A, B, C = towers[ 1 ], towers[ 2 ], towers[ 3 ]    
    local id1, id2, ch = -1, -1
    while true do
      ch = string.char( term.getchar() ):upper()
      if ch == 'Q' then break end
      if ch == '1' or ch == '2' or ch == '3' then
        if id1 == -1 then id1 = tonumber( ch ) else id2 = tonumber( ch ) end
      end
      if id1 ~= -1 and id2 ~= -1 then
        term.moveto( 0, 11 )
        term.clreol()
        if canmove( towers[ id1 ], towers[ id2 ] ) then
          center( id1 .. "->" .. id2, 11 )
          if totower( towers[ id1 ], towers[ id2 ] ) then break end
        else
          center( "Invalid move!", 11 )
        end  
        id1, id2 = -1, -1
      end
      if towers[ 3 ]:_numdisks() == totdisks then 
        center( "Congratulations!", 11 )
        term.getchar()
        break 
      end
    end
  end
end  

term.getchar()
term.clrscr()

