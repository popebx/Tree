#pragma warning(push)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 5275)
#include <gtest/gtest.h>
#pragma warning(pop)
#include <tree/tree.h>
#include <iostream>

template<class data_type>
void print_rec(const tree_node_view<data_type>& view) {
  
}

TEST(tree, creation) {
  tree_node<int> root{5};
  root.add_child(1);
  root.add_child(2);
  root.add_child(3);
  for (auto& child : root.children()) {
    auto another_child = child.add_child(9);
    another_child.add_child(2);
  }
  for (auto child : root.children()) {
    print_rec(child);
  }
  ASSERT_EQ(root.value(), 5);
}
