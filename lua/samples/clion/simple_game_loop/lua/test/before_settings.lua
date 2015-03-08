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

-- data directions
left = 0
up = 1
right = 2
down = 3

-- color id
green = 1

function create_test_data()
  local w = 20
  local h = 20

  register_data_with_params(10, 10, green, w, h, left)
  register_data_with_params(10, 10, green, w, h, left)
  register_data_with_params(10 + w - 1, 10 + h - 1, green, w, h, right)
  register_data_with_params(10 - w, 10 - h, green, w, h, right)
  register_data_with_params(10 - w + 1, 10 - h + 1, green, w, h, right)
end
