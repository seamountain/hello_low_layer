-- Run this at not test but lua directory
require("update")
require("data")
require("data_manager")

-- http://lua-users.org/wiki/UnitTesting
require('test/luaunit/luaunit')

TestUpdate = {}
function TestUpdate:test_turn_opposit_direction()
  local left = 0
  local top = 1
  local right = 2
  local buttom = 3

  d = PointData.new(0, 0, 0, 0, 0, left)
  turn_opposite_direction(d)
  assertEquals(d.d, right)

  d.d = top
  turn_opposite_direction(d)
  assertEquals(d.d, buttom)

  d.d = right
  turn_opposite_direction(d)
  assertEquals(d.d, left)

  d.d = buttom
  turn_opposite_direction(d)
  assertEquals(d.d, top)
end

LuaUnit:run()
