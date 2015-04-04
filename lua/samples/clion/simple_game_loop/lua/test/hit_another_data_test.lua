function TestUpdate:test_hit_same_position_same_size_data()
  local d1 = register_data_with_params(x, y, green, w, h, left)
  local d2 = register_data_with_params(x, y, green, w, h, left)

  local data_index = 1
  hit_another_data(data_index)

  assertEquals(d1.d, right)
  assertEquals(d2.d, left)
end

function TestUpdate:test_hit_top_overlap_data()
  local d1 = register_data_with_params(x, y, green, w, h, left)
  local d2 = register_data_with_params(x - w + 1, y - h + 1, green, w, h, right)

  local data_index = 1
  hit_another_data(data_index)

  assertEquals(d1.d, right)
  assertEquals(d2.d, right)
end

function TestUpdate:test_hit_buttom_overlap_data()
  local d1 = register_data_with_params(x, y, green, w, h, left)
  local d2 = register_data_with_params(x + w - 1, y + h - 1, green, w, h, right)

  local data_index = 1
  hit_another_data(data_index)

  assertEquals(d1.d, right)
  assertEquals(d2.d, right)
end

function TestUpdate:test_hit_same_buttom_and_top_pos_data()
  local d1 = register_data_with_params(x, y, green, w, h, left)
  local d2 = register_data_with_params(x - w, y - h, green, w, h, right)

  local data_index = 1
  hit_another_data(data_index)

  assertEquals(d1.d, right)
  assertEquals(d2.d, right)
end

function TestUpdate:test_not_hit_another_data()
  local d1 = register_data_with_params(x, y, green, w, h, left)
  local d2 = register_data_with_params(x + w + 1, y + h + 1, green, w, h, right)

  local data_index = 1
  hit_another_data(data_index)

  assertEquals(d1.d, left)
  assertEquals(d2.d, right)
end
