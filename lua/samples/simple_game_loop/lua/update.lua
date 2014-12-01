--
-- Created by IntelliJ IDEA.
-- User: sea_mountain
-- Date: 10/13/14
-- Time: 8:05 PM
-- To change this template use File | Settings | File Templates.
--

function move_data_pos(screen_width, screen_height)
    -- direction: 0 left 1 top 2 right 3 buttom

    local data = get_target_data()

    local x = data:get_x()
    local y = data:get_y()
    local width = data:get_width()
    local height = data:get_height()
    local direction = data:get_direction()

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

    local speed = (width * height) / 2
    if direction == 0 then
        x = x - speed
    elseif direction == 1 then
        y = y - speed
    elseif direction == 2 then
        x = x + speed
    elseif direction == 3 then
        y = y + speed
    end

    if is_attacked then
      width = width * 0.9
      height = height * 0.9
      -- TODO delete too small data

      data:set_size(width, height)

      local slide_rate = math.random(890, 950) / 1000
      local new_data = Data(x * slide_rate, y * slide_rate, width, height, direction)
    end

--    print("lua after x " .. x .. " y " .. y .. " direction " .. direction)
    data:update(x, y, direction)
end
