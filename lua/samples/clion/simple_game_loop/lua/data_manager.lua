data_list = {}

function register_data(x, y, color_id)
  local size = math.random(5, 20)
  local w = size
  local h = size
  local dir = math.random(0, 3)
  local d = Data.new(x, y, color_id, w, h, dir)
  table.insert(data_list, d)

  add_to_tree(d)
end

function register_boss_data()
  local x = screen_width / 2
  local y = screen_height / 2
  local w = 100
  local h = 100
  local b = Boss.new(x, y, w, h)
  b.d = math.random(0, 3)

  table.insert(data_list, b)

  add_to_tree(b)
end

function register_data_with_params(x, y, color_id, w, h, dir)
  local d = Data.new(x, y, color_id, w, h, dir)
  table.insert(data_list, d)

  add_to_tree(d)

  return d
end

function data_num()
  return #data_list
end

function hit_another_data(index)
  local d = data_list[index]

  local list_size = #d.data_leaf.p_node
  for i = 1, list_size do
    local target = d.data_leaf.p_node[i].data
    if target ~= d then
      local x1 = d.x
      local w1 = d.w
      local x2 = target.x
      local w2 = target.w

      local y1 = d.y
      local h1 = d.h
      local y2 = target.y
      local h2 = target.h

      local is_attacked = false
      if ((x1 <= x2) and x2 <= (x1 + w1)) and ((y1 <= y2) and (y2 <= (y1 + h1))) then
        is_attacked = true
      elseif (x2 <= x1 and x1 <= (x2 + w2)) and ((y2 <= y1) and y1 <= (y2 + h2)) then
        is_attacked = true
      end

      -- TODO Fix this collision logic. This collisionEnter work with all data and boss but the collisionExit only with a boss.
      if is_attacked then
        d:onCollisionEnter(target)

        -- TODO Delete boss data
        --if d.is_boss then
          --if remove_data(index) then
            --return true
          --end
        --end
      else
        if d.on_boss_collision_enter then
          d:onCollisionExit(target)
        end
      end
    end
  end
  return false
end

-- TODO Add test cheking data_list size after data remove
function move_data()
  local list_size = #data_list
  local i = 1
  while i <= list_size do
    local removed_data_num = move(i, list_size)

    -- The situation data wasn't deleted(data_list size wasn't changed)
    if removed_data_num <= 0 then
      hit_another_data(i)
      update_node(i)
      i = i + 1
    end
    list_size = list_size - removed_data_num
  end
end

function split_data(index)
  local d = data_list[index]
  local shaved_rate = 0.9
  d.w = d.w * shaved_rate
  d.h = d.h * shaved_rate

  local is_removed = remove_data(index)
  if is_removed then
    return 1
  end

  local slide_rate = math.random(890, 950) / 1000
  register_data_with_params(d.x * slide_rate, d.y * slide_rate, d.color_id, d.w, d.h, d.d)
  return 0
end

function remove_data(index)
  local d = data_list[index]
  local min_size = 3
  if d.w <= min_size or d.h <= min_size then
    remove_leaf(d)
    d.data_leaf = nil
    table.remove(data_list, index)
    return true
  end
  return false
end
