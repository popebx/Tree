#pragma once
#include <algorithm>
#include <memory>
#include <vector>

template <class data_type>
class tree_node_view;


template <class data_type>
class tree_node {
 public:
	 
  explicit tree_node(const data_type& value) : m_value{value}, m_children{}, m_parent{nullptr} {}
  tree_node(const tree_node&) = delete;
  explicit tree_node(tree_node&& rhs) : m_value{std::move(rhs.m_value)}, m_children{std::move(rhs.m_children)} {}
  tree_node& operator=(const tree_node&) = delete;
  tree_node& operator=(tree_node&& rhs) {
    this->m_value = std::move(rhs.m_value);
    this->m_children = std::move(rhs.m_children);
    this->parent = std::move(rhs.parent);
    return *this;
  }
  tree_node_view<data_type> add_child(const data_type& value) {
    auto& child = this->m_children.emplace_back(new tree_node{value, this});
    return tree_node_view<data_type> {child.get()};
  }
  data_type& value() { return this->m_value; }
  const data_type& value() const { return this->m_value; }
  bool is_leaf_node() const { return this->m_children.empty(); }
  bool is_root_node() const { return this->parent == nullptr; }
  std::vector<tree_node_view<data_type>> children() {
    std::vector<tree_node_view<data_type>> children_view{};
    children_view.reserve(this->m_children.size());
    for (const std::unique_ptr<tree_node>& child : m_children) {
      children_view.emplace_back(child.get());
    }
    return std::move(children_view);
  }

 private:
  data_type m_value;
  std::vector<std::unique_ptr<tree_node>> m_children;
  tree_node* m_parent = nullptr;

  tree_node(const data_type& value, tree_node<data_type>* parent) : m_value{value}, m_children{}, m_parent{parent} {}
  void replace_child(tree_node* old_child, tree_node* new_child) {
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
  explicit tree_node_view(tree_node<data_type>* ptr) : m_view{ptr} {}
  data_type& value() { return this->m_view->value(); }
  const data_type& value() const { return this->m_view->value(); }
  bool is_leaf_node() const { return this->m_view->m_children.empty(); }
  bool is_root_node() const { return this->m_view->parent == nullptr; }
  std::vector<tree_node_view<data_type>> children() { return this->m_view->children(); }
  tree_node_view<data_type> add_child(const data_type& value) { return this->m_view->add_child(value); }
  std::vector<tree_node_view> children() const { return this->m_view->children(); }
 private:
  tree_node<data_type>* m_view;
};