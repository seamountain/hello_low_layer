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
    return
  end

  -- TODO Fix storing invalid data at data_list[index]
  if data_list[index] == nil then
    return
  end

  d:move_position()

  if is_attacked and d.is_split then
    split_data(index)
  end
end

function check_wall_collision(d)
  local is_attacked = true
  local wall_pos_dir = -1
  if screen_width - d.w < d.x then
    wall_pos_dir = right
  elseif d.x <= 0 then
    wall_pos_dir = left
  elseif screen_height - d.h < d.y then
    wall_pos_dir = buttom
  elseif d.y <= 0 then
    wall_pos_dir = top
  else
    is_attacked = false
  end
  return is_attacked, wall_pos_dir
end

function reset_potision_inside_wall(d, wall_pos_dir)
  if wall_pos_dir == left then
    d.x = 0
  elseif wall_pos_dir == top then
    d.y = 0
  elseif wall_pos_dir == right then
    d.x = screen_width - d.w
  elseif wall_pos_dir == buttom then
    d.y = screen_height - d.h
  end
end

function check_boss_distance(d)
  -- TODO Add test
  local is_near_boss = false
  local boss_pos_dir = -1

  if (boss_data.y < d.y and d.y < (boss_data.y + boss_data.h)) then
    if (boss_data.x - d.attack_range) < (d.x + d.w) and d.x < (boss_data.x + boss_data.w) then
      is_near_boss = true
      boss_pos_dir = right
    elseif d.x < (boss_data.x + boss_data.w + d.attack_range) and (boss_data.x - d.attack_range) < d.x then
      is_near_boss = true
      boss_pos_dir = left
    end
  elseif ((boss_data.x < d.x and d.x < (boss_data.x + boss_data.w))) then
    if (boss_data.y - d.attack_range) < (d.y + d.h) and d.y < (boss_data.y + boss_data.h) then
      is_near_boss = true
      boss_pos_dir = down
    elseif d.y < (boss_data.y + boss_data.h + d.attack_range) and (boss_data.y - d.attack_range) < d.y then
      is_near_boss = true
      boss_pos_dir = top
    end
  end
  return is_near_boss, boss_pos_dir
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
