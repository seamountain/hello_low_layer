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

  return obj
end
