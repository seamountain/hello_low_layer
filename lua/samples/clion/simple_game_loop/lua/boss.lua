Boss = {}
Boss.new = function (x, y, w, h, ar)
  local obj = {}
  obj.x = x
  obj.y = y
  obj.color_id = -1
  obj.w = w
  obj.h = h
  obj.d = -1
  obj.attack_range = ar
  obj.data_leaf = {}
  obj.is_boss = true -- Replace this frag with checking class
  obj.is_move = true
  obj.is_split = false
  obj.move_frame_count = 0
  obj.keep_same_direction_count = 50

  obj.move_position = function (self)
    local is_attacked = check_wall_collision(self)
    if is_attacked then
      turn_opposite_direction(self)
    end

    local speed = (self.w * self.h) / 1000 / 2
    if self.d == left then
      self.x = self.x - speed
    elseif self.d == up then
      self.y = self.y - speed
    elseif self.d == right then
      self.x = self.x + speed
    elseif self.d == down then
      self.y = self.y + speed
    end

    if self.keep_same_direction_count < self.move_frame_count then
      self.d = math.random(0, 3)
      self.move_frame_count = 0
    else
      self.move_frame_count = self.move_frame_count + 1
    end

    boss_data = self
  end

  obj.onCollisionEnter = function (self)
    set_boss_texture_flag(false)
    if 0 <= self.w or 0 <= self.h then
      shrink_size = 0.1
      self.w = self.w - shrink_size
      self.h = self.h - shrink_size
    end
  end

  obj.onCollisionExit = function (self)
    set_boss_texture_flag(true)
  end

  setmetatable(obj, {__index=Data})

  return obj
end
