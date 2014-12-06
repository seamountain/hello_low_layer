Data = {}
Data.new = function (x, y)
    local obj = {}
    obj.x = x
    obj.y = y

    obj.print = function (this)
        print("Data.lua x: " .. this.x .. ", y: " .. this.y)
    end

    return obj
end

local data1 = Data.new(0, 10)
local data2 = Data.new(0, 20)
local data3 = Data.new(0, 30)

local data_list = { data1, data2, data3 }

for i = 1, #data_list, 1 do
  data_list[i]:print()
end
