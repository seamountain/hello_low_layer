--
-- Created by IntelliJ IDEA.
-- User: sea_mountain
-- Date: 10/13/14
-- Time: 8:05 PM
-- To change this template use File | Settings | File Templates.
--

function move_data_pos(x, y, width, height, direction, screen_width, screen_height)
    -- direction: 0 left 1 top 2 right 3 buttom
--    print("lua before x " .. x .. " y " .. y .. " direction " .. direction)

    if x <= 0 then
        direction = 2
    elseif screen_width - width < x then
        direction = 0
    elseif y < 0 then
        direction = 3
    elseif screen_height - height < y then
        direction = 1
    end

    local speed = 5 * (width * height) / 100
    if direction == 0 then
        x = x - speed
    elseif direction == 1 then
        y = y - speed
    elseif direction == 2 then
        x = x + speed
    elseif direction == 3 then
        y = y + speed
    end

--    print("lua after x " .. x .. " y " .. y .. " direction " .. direction)
    return x, y, direction
end