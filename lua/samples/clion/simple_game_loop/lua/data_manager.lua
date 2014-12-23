data_list = {}
function register_data(x, y, color_id)
  local w = 10
  local h = 10
  local dir = math.random(0, 3)
  local d = Data.new(x, y, color_id, w, h, dir)
  table.insert(data_list, d)
end

function data_num()
  return #data_list
end

function hit_another_data(index)
  local d = data_list[index]
  local list_size = #data_list
  for i = 1, list_size do
    local target = data_list[i]
    if target ~= d then
      local x1 = d.x
      local w1 = d.w
      local x2 = target.x
      local w2 = target.w

      local y1 = d.y
      local h1 = d.h
      local y2 = target.y
      local h2 = target.h

      if ((x1 - w2 <= x2 and x2 <= x1 + w1) and (y1 - h2 <= y2 and y2 <= y1 + h1)) then

        -- TODO Add splitting data
        turn_opposite_direction(d)
        turn_opposite_direction(target)

        return 0
      end
    end
  end

  return 0
end

-- TODO data_list size test afte removing
function move_data()
  local list_size = #data_list
  for i = 1, list_size do
    local attacked_wall_data = move(i)
    -- TODO Fix removed data list index
    local attacked_another_data = hit_another_data(i)

    local removed_data_sum = attacked_wall_data + attacked_another_data
    i = i -removed_data_sum

    list_size = list_size - removed_data_sum
  end
end

function split_data(index)
  local d = data_list[index]
  local shaved_rate = 0.9
  d.w = d.w * shaved_rate
  d.h = d.h * shaved_rate

  --TODO Remove small data
  local min_size = 2
  if d.w <= min_size or d.h <= min_size then
    table.remove(data_list, index)
    return true
  end

  local slide_rate = math.random(890, 950) / 1000
  local splited_data = Data.new(d.x * slide_rate, d.y * slide_rate, d.color_id, d.w, d.h, d.d)
  table.insert(data_list, splited_data)

  return false
end
