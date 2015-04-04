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

-- default x/y
x = 10
y = 10

-- default width/height
w = 20
h = 20

function TestUpdate:tearDown()
  clear_data()
end

function clear_data()
  data_list = {}
end
