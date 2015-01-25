space_tree = {
  {{
    {}, {}, {}, {}
  },
  {
    {}, {}, {}, {}
  }},
  {{
    {}, {}, {}, {}
  },
  {
    {}, {}, {}, {}
  }},
}

-- 木構造イメージ図
--      space_tree
--      a        b
--    c   d    e   f
--  ghij klmn opqr stuv

node_a = space_tree[1]
node_b = space_tree[2]
node_c = node_a[1]
node_d = node_a[2]
node_e = node_b[1]
node_f = node_b[2]

node_g = node_c[1]
node_h = node_c[2]
node_i = node_c[3]
node_j = node_c[4]

node_k = node_d[1]
node_l = node_d[2]
node_m = node_d[3]
node_n = node_d[4]

node_o = node_e[1]
node_p = node_e[2]
node_q = node_e[3]
node_r = node_e[4]

node_s = node_f[1]
node_t = node_f[2]
node_u = node_f[3]
node_v = node_f[4]

function add_to_tree(d)
  local leaf = DataLeaf.new(d)
  d.data_leaf = leaf

  local target_node = get_target_space(d)

  table.insert(target_node, leaf)
  d.data_leaf.p_node = target_node

  leaf.index = #d.data_leaf.p_node
end

function update_node(index)
  -- TODO 境界位置にある時、複数の空間に属すように変更
  local d = data_list[index]
  local target_node = get_target_space(d)

  if d.data_leaf.p_node ~= target_node then
    remove_leaf(d)
    table.insert(target_node, d.data_leaf)
    d.data_leaf.index = #target_node
    d.data_leaf.p_node = target_node
  end

  --print(string.format(
  --"g %s, h %s, i %s, j %s,  k %s, l %s, m %s, n %s,  o %s, p %s, q %s, r %s,  s %s, t %s, u %s, v %s",
  --#node_g, #node_h, #node_i, #node_j, #node_k, #node_l, #node_m, #node_n, #node_o, #node_p, #node_q, #node_r, #node_s, #node_t, #node_u, #node_v))
end

function remove_leaf(d)
  local index = -1
  for i = 1, #d.data_leaf.p_node do
    if d.data_leaf.p_node[i] == d.data_leaf then
      index = i
      break
    end
  end
  table.remove(d.data_leaf.p_node, index)
end

-- TODO もっと良い方法に直す
function get_target_space(d)
  local target_node = {}
  if d.x < screen_width / 2 then -- A
    if d.y < screen_height / 2 then -- C
      if d.x < screen_width / 4 then
        if d.y < screen_height / 4 then
          target_node = node_g
        else
          target_node = node_h
        end
      else
        if d.y < screen_height / 4 then
          target_node = node_i
        else
          target_node = node_j
        end
      end
    else -- D
      if d.x < screen_width / 4 then
        if d.y < screen_height / 2 + screen_height / 4 then
          target_node = node_k
        else
          target_node = node_l
        end
      else
        if d.y < screen_height / 2 + screen_height / 4 then
          target_node = node_m
        else
          target_node = node_n
        end
      end
    end
  else -- B
    if d.y < screen_height / 2 then -- E
      if d.x < screen_height / 2 + screen_width / 4 then
        if d.y < screen_height / 4 then
          target_node = node_o
        else
          target_node = node_p
        end
      else
        if d.y < screen_height / 4 then
          target_node = node_q
        else
          target_node = node_r
        end
      end
    else -- F
      if d.x < screen_height / 2 + screen_width / 4 then
        if d.y < screen_height / 2 + screen_height / 4 then
          target_node = node_s
        else
          target_node = node_t
        end
      else
        if d.y < screen_height / 2 + screen_height / 4 then
          target_node = node_u
        else
          target_node = node_v
        end
      end
    end
  end

  return target_node
end
