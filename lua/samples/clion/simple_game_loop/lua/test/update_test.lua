-- Run this at not test but lua directory
require("update")

-- http://lua-users.org/wiki/UnitTesting
require('test/luaunit/luaunit')

TestUpdate = {}
function TestUpdate:test_turn_opposit_direction()
  left = 0
  top = 1
  right = 2
  buttom = 3

  assertEquals(turn_opposite_direction(left), right)
  assertEquals(turn_opposite_direction(top), buttom)
  assertEquals(turn_opposite_direction(right), left)
  assertEquals(turn_opposite_direction(buttom), top)
end

LuaUnit:run()
