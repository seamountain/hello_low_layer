function call_c()
  local r = CFunc()
  print("Return value from C", r)
  print("This Lua ver", _VERSION)
end
