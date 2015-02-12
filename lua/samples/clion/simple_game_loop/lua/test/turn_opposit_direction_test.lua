function TestUpdate:test_turn_opposit_direction()
  local left = 0
  local top = 1
  local right = 2
  local buttom = 3

  d = Data.new(0, 0, 0, 0, 0, left)
  turn_opposite_direction(d)
  assertEquals(d.d, right)

  d.d = top
  turn_opposite_direction(d)
  assertEquals(d.d, buttom)

  d.d = right
  turn_opposite_direction(d)
  assertEquals(d.d, left)

  d.d = buttom
  turn_opposite_direction(d)
  assertEquals(d.d, top)
end
