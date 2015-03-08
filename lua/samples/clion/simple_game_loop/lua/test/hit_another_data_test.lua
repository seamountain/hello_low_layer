function TestUpdate:test_hit_another_data()
  create_test_data()

  local data_index = 1
  hit_another_data(data_index)

  local d1 = data_list[1]
  local d2 = data_list[2]
  local d3 = data_list[3]
  local d4 = data_list[4]
  local d5 = data_list[5]

  assertEquals(d1.d, right)
  assertEquals(d2.d, left)
  assertEquals(d3.d, left)
  assertEquals(d4.d, right)
  assertEquals(d5.d, right)
end

function before_not_hit_another_data()
  local d1 = data_list[1]
  local d2 = data_list[2]

  d2.x = d1.x + d1.w + 1
  d2.y = d1.y + d1.h + 1

  d1.d = left
  d2.d = right
end

function TestUpdate:test_not_hit_another_data()
  before_not_hit_another_data()

  local data_index = 1
  hit_another_data(data_index)

  local d1 = data_list[1]
  local d2 = data_list[2]

  assertEquals(d1.d, left)
  assertEquals(d2.d, right)
end
