function TestUpdate:test_split_data()
  local d = register_data_with_params(10, 10, green, w, h, left)
  local data_index = data_num()
  split_data(data_index)

  local shaved_rate = 0.9
  assertEquals(d.w, w * shaved_rate)
  assertEquals(d.h, h * shaved_rate)
end
