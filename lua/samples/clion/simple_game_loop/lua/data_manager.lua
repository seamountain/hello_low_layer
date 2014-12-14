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

function hit_another_data(d)
  for i = 1, #data_list, 1 do
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
        turn_opposite_direction(target)
        move(target)
      end
    end
  end
end

function move_data()
  for i = 1, #data_list, 1 do
    local d = data_list[i]
    move(d)
    hit_another_data(d)
  end
end
