Boss = {}
Boss.new = function (x, y, w, h, d)
  local obj = {}
  obj.x = x
  obj.y = y
  obj.color_id = -1
  obj.w = w
  obj.h = h
  obj.d = d
  obj.data_leaf = {}
  obj.is_move = false

  obj.onCollisionEnter = function (self)
    set_boss_texture_flag(false)
  end

  obj.onCollisionExit = function (self)
    set_boss_texture_flag(true)
  end

  setmetatable(obj, {__index=Data})

  return obj
end
