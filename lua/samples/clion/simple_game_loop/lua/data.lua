Data = {}
Data.new = function (x, y, color_id, w, h, d)
  local obj = {}
  obj.x = x
  obj.y = y
  obj.color_id = color_id
  obj.w = w
  obj.h = h
  obj.d = d
  obj.data_leaf = {}
  obj.is_move = true

  obj.onCollisionEnter = function (self, target)
    reset_potision(self, target)
    print("ENTER")
    self:onCollisionExit() -- TODO Fix called timing
  end

  obj.onCollisionExit = function (self)
    print("EXIT")
  end

  return obj
end
