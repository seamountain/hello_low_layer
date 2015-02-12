-- README: Run this at lua directory
-- lua test/test_manager.lua

require("test/before_settings")

require("test/turn_opposit_direction_test")
require("test/hit_another_data_test")
require("test/split_data_test")

LuaUnit:run()
