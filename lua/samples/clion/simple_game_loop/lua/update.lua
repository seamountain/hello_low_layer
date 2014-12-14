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

function move(d)
  --local is_attacked = true

  if screen_width - d.w < d.x then
    d.d = 0
  elseif d.x <= 0 then
    d.d = 2
  elseif screen_height - d.h < d.y then
    d.d = 1
  elseif d.y <= 0 then
    d.d = 3
  else
    --is_attacked = false
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

  -- TODO 固まるのを直す
  --if is_attacked then
  --width = width * 0.9
  --height = height * 0.9
  ---- TODO delete too small data

  --set_size(width, height)

  --local slide_rate = math.random(890, 950) / 1000
  --local new_data = Data(x * slide_rate, y * slide_rate, width, height, direction)
  --end

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
