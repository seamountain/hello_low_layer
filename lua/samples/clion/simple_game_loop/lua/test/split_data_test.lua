function TestUpdate:test_split_data()
  create_test_data()

  local index = 1
  local orig_data_w = data_list[index].w
  local orig_data_h = data_list[index].h

  split_data(index)
  local updated_data = data_list[index]

  local shaved_rate = 0.9
  assertEquals(updated_data.w, orig_data_w * shaved_rate)
  assertEquals(updated_data.h, orig_data_h * shaved_rate)
end
