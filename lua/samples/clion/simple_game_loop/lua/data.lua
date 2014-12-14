PointData = {}
PointData.new = function (x, y, color_id, w, h, d)
  local obj = {}
  obj.x = x
  obj.y = y
  obj.color_id = color_id
  obj.w = w
  obj.h = h
  obj.d = d

  return obj
end
