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
    if target ~= nil and target.is_move == false then
      target:onCollisionEnter(d)
    end

    reset_potision(self, target)

    self:onCollisionExit(target) -- TODO Fix called timing
  end

  obj.onCollisionExit = function (self, target)
    if target ~= nil and target.is_move == false then
      target:onCollisionExit()
    end
  end

  return obj
end
