--
-- Created by IntelliJ IDEA.
-- User: sea_mountain
-- Date: 10/13/14
-- Time: 8:05 PM
-- To change this template use File | Settings | File Templates.
--

screen_width = 0
screen_height = 0

function set_screen_size(sw, sh)
  screen_width = sw
  screen_height = sh
end

function move(index)
  local d = data_list[index]

  if d == nil then
    return
  end
  local is_attacked = true

  if screen_width - d.w < d.x then
    d.d = 0
  elseif d.x <= 0 then
    d.d = 2
  elseif screen_height - d.h < d.y then
    d.d = 1
  elseif d.y <= 0 then
    d.d = 3
  else
    is_attacked = false
  end

  local speed = (d.w * d.h) / 8
  if d.d == 0 then
    d.x = d.x - speed
  elseif d.d == 1 then
    d.y = d.y - speed
  elseif d.d == 2 then
    d.x = d.x + speed
  elseif d.d == 3 then
    d.y = d.y + speed
  end

  if is_attacked then
    split_data(index)
  end
end

function turn_opposite_direction(d)
  if d.d == 0 then
    d.d = 2
  elseif d.d == 1 then
    d.d = 3
  elseif d.d == 2 then
    d.d = 0
  elseif d.d == 3 then
    d.d = 1
  end
end
