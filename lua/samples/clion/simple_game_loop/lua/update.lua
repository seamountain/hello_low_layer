--
-- Created by IntelliJ IDEA.
-- User: sea_mountain
-- Date: 10/13/14
-- Time: 8:05 PM
-- To change this template use File | Settings | File Templates.
--

screen_width = 0
screen_height = 0

function move_data_pos(sw, sh)
    -- direction: 0 left 1 top 2 right 3 buttom
    screen_width = sw
    screen_height = sh

    local x = get_x()
    local y = get_y()
    local width = get_width()
    local height = get_height()
    local direction = get_direction()

    local hit_data = get_hit_data()

    if hit_data ~= nil then
      move_with_hit_data(hit_data)
      direction = turn_opposite_direction(direction)
    end
    x, y, direction = move(x, y, width, height, direction)

--    print("lua after x " .. x .. " y " .. y .. " direction " .. direction)
    update_data(x, y, direction)
end

function move(x, y, width, height, direction)
    local is_attacked = true

    if screen_width - width < x then
        direction = 0
    elseif x <= 0 then
        direction = 2
    elseif screen_height - height < y then
        direction = 1
    elseif y <= 0 then
        direction = 3
    else
        is_attacked = false
    end

    local speed = (width * height) / 8
    if direction == 0 then
        x = x - speed
    elseif direction == 1 then
        y = y - speed
    elseif direction == 2 then
        x = x + speed
    elseif direction == 3 then
        y = y + speed
    end

    -- TODO 固まるのを直す
    --if is_attacked then
      --width = width * 0.9
      --height = height * 0.9
      ---- TODO delete too small data

      --set_size(width, height)

      --local slide_rate = math.random(890, 950) / 1000
      --local new_data = Data(x * slide_rate, y * slide_rate, width, height, direction)
    --end

    return x, y, direction
end

function move_with_hit_data(d)
  local x2 = d:get_x()
  local y2 = d:get_y()
  local w2 = d:get_width()
  local h2 = d:get_height()
  local d2 = turn_opposite_direction(d:get_direction())
  x2, y2, d2 = move(x2, y2, w2, h2, d2)

  d:update(x2, y2, d2)
end

function turn_opposite_direction(d)
  if d == 0 then
    d = 2
  elseif d == 1 then
    d = 3
  elseif d == 2 then
    d = 0
  elseif d == 3 then
    d = 1
  end

  return d
end
