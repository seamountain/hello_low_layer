--
-- Created by IntelliJ IDEA.
-- User: sea_mountain
-- Date: 10/13/14
-- Time: 8:05 PM
-- To change this template use File | Settings | File Templates.
--

function add_data()
    -- x, y, w, h
    data_size = 4
    data_table = ""

    for i = 1, data_size do
        data_table = data_table .. math.random(300)
        -- add delimiter
        if (i ~= data_table) then
            data_table = data_table .. " "
        end
    end

    return data_table;
end


