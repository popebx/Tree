#pragma once
#include <algorithm>
#include <memory>
#include <vector>

namespace container::tree {
template <class data_type>
class tree_node_view;

template <class data_type>
class tree {
 public:
  explicit tree(const data_type& value) : m_value{value}, m_children{}, m_parent{nullptr} {}
  tree(const tree&) = delete;
  explicit tree(tree&& rhs) : m_value{std::move(rhs.m_value)}, m_children{std::move(rhs.m_children)} {}
  tree& operator=(const tree&) = delete;
  tree& operator=(tree&& rhs) {
    this->m_value = std::move(rhs.m_value);
    this->m_children = std::move(rhs.m_children);
    this->parent = std::move(rhs.parent);
    return *this;
  }

  operator tree_node_view<data_type>() { return tree_node_view<data_type>(this); }

  tree_node_view<data_type> add_child(const data_type& value) {
    this->m_children.emplace_back(new tree{value, this});
    return tree_node_view<data_type>{this->m_children.back().get()};
  }

  data_type& value() { return this->m_value; }
  const data_type& value() const { return this->m_value; }
  bool is_leaf_node() const { return this->m_children.empty(); }
  bool is_root_node() const { return this->parent == nullptr; }
  std::vector<tree_node_view<data_type>> children() {
    std::vector<tree_node_view<data_type>> children_view{};
    children_view.reserve(this->m_children.size());
    for (const std::unique_ptr<tree>& child : m_children) {
      children_view.emplace_back(child.get());
    }
    return std::move(children_view);
  }

 private:
  data_type m_value;
  std::vector<std::unique_ptr<tree>> m_children;
  tree* m_parent = nullptr;

  tree(const data_type& value, tree<data_type>* parent) : m_value{value}, m_children{}, m_parent{parent} {}
  void replace_child(tree* old_child, tree* new_child) {
    for (auto it = this->m_children.begin(); it != this->m_children.end(); it++) {
      if (it->get() == old_child) {
        it->reset();
        it = std::unique_ptr{new_child};
      }
    }
    std::make_unique
  }
};

template <class data_type>
class tree_node_view {
 public:
  explicit tree_node_view(tree<data_type>* ptr) : m_view{ptr} {}
  data_type& value() { return this->m_view->value(); }
  const data_type& value() const { return this->m_view->value(); }
  bool is_leaf_node() const { return this->m_view->m_children.empty(); }
  bool is_root_node() const { return this->m_view->parent == nullptr; }
  std::vector<tree_node_view<data_type>> children() { return this->m_view->children(); }
  tree_node_view<data_type> add_child(const data_type& value) { return this->m_view->add_child(value); }
  std::vector<tree_node_view> children() const { return this->m_view->children(); }

 private:
  tree<data_type>* m_view;
};
}  // namespace container::tree