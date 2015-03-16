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
  obj.is_on_collision_enter = false

  obj.onCollisionEnter = function (self, target)
    if target ~= nil and target.is_move == false then
      target:onCollisionEnter()
    end

    turn_opposite_direction(self)
    self.on_collision_enter = true
    --reset_potision(self, target)
  end

  obj.onCollisionExit = function (self, target)
    if target ~= nil and target.is_move == false then
      target:onCollisionExit()
    end
    self.on_collision_enter = false
  end

  return obj
end
