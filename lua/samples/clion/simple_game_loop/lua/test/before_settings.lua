require("update")
require("data")
require("data_manager")
require("data_leaf")
require("space_pertition")

-- http://lua-users.org/wiki/UnitTesting
require('test/luaunit/luaunit')

TestUpdate = {}

screen_width = 640
screen_height = 640
left = 0
right = 2

function create_test_data()
  local w = 20
  local h = 20

  register_data_with_params(10, 10, 1, w, h, left)
  register_data_with_params(10, 10, 1, w, h, right)
end
