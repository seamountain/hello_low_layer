--
-- Created by IntelliJ IDEA.
-- User: sea_mountain
-- Date: 10/13/14
-- Time: 8:05 PM
-- To change this template use File | Settings | File Templates.
--

screen_width = 0
screen_height = 0

left = 0
up = 1
right = 2
down = 3

function set_screen_size(sw, sh)
  screen_width = sw
  screen_height = sh
end

function move(index, list_size)
  local d = data_list[index]

  if d.is_move == false then
    return 0
  end

  -- TODO Fix storing invalida data at data_list[index]
  if data_list[index] == nil then
    return 0
  end

  local is_attacked = true

  if screen_width - d.w < d.x then
    d.d = left
  elseif d.x <= 0 then
    d.d = right
  elseif screen_height - d.h < d.y then
    d.d = up
  elseif d.y <= 0 then
    d.d = down
  else
    is_attacked = false
  end

  local speed = (d.w * d.h) / 10
  if d.d == left then
    d.x = d.x - speed
  elseif d.d == up then
    d.y = d.y - speed
  elseif d.d == right then
    d.x = d.x + speed
  elseif d.d == down then
    d.y = d.y + speed
  end

  if is_attacked then
    return split_data(index)
  end

  return 0
end

function turn_opposite_direction(d)
  if d.d == left then
    d.d = right
  elseif d.d == up then
    d.d = down
  elseif d.d == right then
    d.d = left
  elseif d.d == down then
    d.d = up
  end
end

function reset_potision(d, target)
  local x1 = d.x
  local w1 = d.w
  local x2 = target.x
  local w2 = target.w

  local y1 = d.y
  local h1 = d.h
  local y2 = target.y
  local h2 = target.h

  if d.d == left then
    d.x = x1 + ((x2 + w2) - x1)
  elseif d.d == right then
    d.x = x1 - ((x1 + w1) - x2)
  elseif d.d == up then
    d.y = y1 + ((y2 + h2) - y1)
  elseif d.d == down then
    d.y = y1 - ((y1 + h1) - y2)
  end

  turn_opposite_direction(d)
  turn_opposite_direction(target)
  --print("d.x " .. d.x .. " d.y " .. d.y .. " d.d " .. d.d)
end
