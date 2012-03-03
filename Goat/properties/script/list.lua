-----------------------------------------------------------------------
-- for global scripts; switch to "GoatPropertiesHome" for per-user scripts
-----------------------------------------------------------------------
function loadscript(scriptfile)
  dofile(props["GoatPropertiesHome"].."/script/"..scriptfile)
end

-----------------------------------------------------------------------
-- run selected scripts, silently fails if no extman
-----------------------------------------------------------------------
-- [[
if scite_Command then
scite_Command('Run A Script|ChooseAScript|Ctrl+l')
local AppList = {
  {"Tic Tac Toe", "GoatTicTacToe.lua", "TicTacToe"},
  {"Classic Eliza", "GoatElizaClassic.lua", "ClassicEliza"},
}
function ChooseAScript()
  local list = {}
  for i,v in ipairs(AppList) do list[i] = v[1] end
  if scite_UserListShow then
    scite_UserListShow(list, 1, RunSelectedScript)
  end
end
function RunSelectedScript(str)
  for i,v in ipairs(AppList) do
    if str == v[1] then
      loadscript(v[2]) -- change this to suit your environment
      if type(_G[v[3]]) == "function" then _G[v[3]]() end
    end
  end
end
end
--]]
