--
-- Created by IntelliJ IDEA.
-- User: sea_mountain
-- Date: 10/13/14
-- Time: 8:05 PM
-- To change this template use File | Settings | File Templates.
--

local direction
function move_ball(x, y, width, height, screen_width, screen_height)
    -- direction: 0 left 1 top 2 right 3 buttom

    if x <= 0 then
        direction = 2
    elseif screen_width - width < x then
        direction = 0
    elseif y < 0 then
        direction = 3
    elseif screen_height - height < y then
        direction = 1
    end

    local speed = 5 * (width * height) / 10000
    if direction == 0 then
        x = x - speed
    elseif direction == 1 then
        y = y - speed
    elseif direction == 2 then
        x = x + speed
    elseif direction == 3 then
        y = y + speed
    end

    print(direction)
    print(x)
    print(screen_width)
    register_ball_pos(x, y)
end