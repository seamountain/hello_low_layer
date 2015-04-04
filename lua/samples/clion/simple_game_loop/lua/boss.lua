Boss = {}
Boss.new = function (x, y, w, h)
  local obj = {}
  obj.x = x
  obj.y = y
  obj.color_id = -1
  obj.w = w
  obj.h = h
  obj.d = -1
  obj.data_leaf = {}
  obj.is_move = true

  obj.onCollisionEnter = function (self)
    set_boss_texture_flag(false)
  end

  obj.onCollisionExit = function (self)
    set_boss_texture_flag(true)
  end

  setmetatable(obj, {__index=Data})

  return obj
end
