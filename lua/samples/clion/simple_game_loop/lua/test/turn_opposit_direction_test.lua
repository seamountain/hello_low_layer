function TestUpdate:test_turn_opposit_direction()
  d = Data.new(0, 0, 0, 0, 0, left)
  turn_opposite_direction(d)
  assertEquals(d.d, right)

  d.d = up
  turn_opposite_direction(d)
  assertEquals(d.d, down)

  d.d = right
  turn_opposite_direction(d)
  assertEquals(d.d, left)

  d.d = down
  turn_opposite_direction(d)
  assertEquals(d.d, up)
end
