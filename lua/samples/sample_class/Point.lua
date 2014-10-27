-- http://www.hakkaku.net/articles/20081118-286

Point = {}
Point.new = function (x, y)
    local obj = {}
    obj.x = x
    obj.y = y

    obj.moveX = function (this)
        this.x = this.x + pos
    end

    obj.moveY = function (this)
        this.y = this.y + pos
    end

    obj.print = function (this)
        print("Point.lua x: " .. this.x .. ", y: " .. this.y)
    end

    return obj
end


p = Point.new(10, 20);
p:print();