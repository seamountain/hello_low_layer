Data = {}
Data.new = function (x, y, color_id, w, h, d, ar)
  local obj = {}
  obj.x = x
  obj.y = y
  obj.color_id = color_id
  obj.w = w
  obj.h = h
  obj.d = d
  obj.attack_range = ar
  obj.data_leaf = {}
  obj.is_move = true
  obj.is_split = true
  -- TODO fix enter/exit state handling logic
  obj.on_boss_collision_enter = false

  obj.move_position = function (self)
    local is_near_boss, boss_pos_dir = check_boss_distance(self)

    if is_near_boss then
      self.d = boss_pos_dir
      turn_opposite_direction(self)
    end

    if is_near_boss then
      local speed = (self.w * self.h) / 10
      if self.d == left then
        self.x = self.x - speed
      elseif self.d == up then
        self.y = self.y - speed
      elseif self.d == right then
        self.x = self.x + speed
      elseif self.d == down then
        self.y = self.y + speed
      end
    end

    local is_attacked, wall_pos_dir = check_wall_collision(self)
    if is_attacked then
      turn_opposite_direction(self)
      reset_potision_inside_wall(self, wall_pos_dir)
    end
  end

  obj.onCollisionEnter = function (self, target)
    if target.is_boss then
      target:onCollisionEnter()
      self.on_boss_collision_enter = true
    end

    turn_opposite_direction(self)
    --reset_potision(self, target)
  end

  obj.onCollisionExit = function (self, target, index)
    if target.is_boss then
      target:onCollisionExit()
      self.on_boss_collision_enter = false
      remove_data_immediately(index)
    end
  end

  return obj
end
