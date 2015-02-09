-- Run this at not test but lua directory
require("update")
require("data")
require("data_manager")
require("data_leaf")
require("space_pertition")

-- http://lua-users.org/wiki/UnitTesting
require('test/luaunit/luaunit')

-- TODO Add before method

left = 0
right = 2
function before_split_data()
  local w = 20
  local h = 20

  register_data_with_params(10, 10, 1, w, h, left)
  register_data_with_params(10, 10, 1, w, h, right)
end

TestUpdate = {}
function TestUpdate:test_split_data()
  before_split_data()

  local index = 1
  local orig_data_w = data_list[index].w
  local orig_data_h = data_list[index].h

  split_data(index)
  local updated_data = data_list[index]

  local shaved_rate = 0.9
  assertEquals(updated_data.w, orig_data_w * shaved_rate)
  assertEquals(updated_data.h, orig_data_h * shaved_rate)
end

LuaUnit:run()
