Boss = {}
Boss.new = function (x, y, color_id, w, h, d)
  local obj = {}
  obj.x = x
  obj.y = y
  obj.color_id = color_id
  obj.w = w
  obj.h = h
  obj.d = d
  obj.data_leaf = {}
  obj.is_move = false

  obj.onCollisionEnter = function (self)
    --print("ENTER")
  end

  obj.onCollisionExit = function (self)
    --print("EXIT")
  end

  setmetatable(obj, {__index=Data})

  return obj
end
