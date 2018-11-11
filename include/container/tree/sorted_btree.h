#pragma once
#include <memory>
#include <optional>
#include <set>
#include <type_traits>

namespace container::tree {
template <class data_type>
class sorted_btree_view;

template <class data_type>
class sorted_btree_const_view;

template <class data_type>
class sorted_btree {
 public:
  sorted_btree() = delete;
  sorted_btree(const data_type& val) : m_value{val} {}
  ~sorted_btree() {
    if (this->m_parent == nullptr) {
      auto* current_element = this;
      while (current_element) {
        if (current_element->left_node == nullptr && current_element->right_node == nullptr) {
          auto element_to_delete = current_element;
          current_element = current_element->m_parent;
          if (current_element != nullptr) {
            if (current_element->left_node.get() == element_to_delete) {
              current_element->left_node.reset();
            } else {
              current_element->right_node.reset();
            }
          }
        } else if (current_element->left_node != nullptr) {
          current_element = current_element->left_node.get();
        } else if (current_element->right_node != nullptr) {
          current_element = current_element->right_node.get();
        }
      }
    }
  };
  sorted_btree(const sorted_btree&) = delete;
  sorted_btree(sorted_btree&& rhs) {
    m_value = std::move(rhs.m_value);
    left_node = std::move(rhs.left_node);
    right_node = std::move(rhs.right_node);
    if (this->has_left()) {
      this->left_node->parent = this;
    }
    if (this->has_right()) {
      this->right_node->parent = this;
    }
  }
  operator sorted_btree_view<data_type>() { return sorted_btree_view<data_type>{this}; }
  operator sorted_btree_const_view<data_type>() const { return sorted_btree_const_view<data_type>{this}; }
  bool is_root() const { return this->m_parent == null; }
  void insert(const data_type& value) {
    auto* current_element = this;
    while (current_element) {
      if (current_element->m_value == value) {
        return;
      } else if (value < current_element->m_value) {
        if (current_element->left_node == nullptr) {
          current_element->left_node = std::unique_ptr<sorted_btree>{new sorted_btree<data_type>(value, current_element)};
          return;
        } else {
          current_element = current_element->left_node.get();
        }
      } else if (current_element->m_value < value) {
        if (current_element->right_node == nullptr) {
          current_element->right_node = std::unique_ptr<sorted_btree>{new sorted_btree<data_type>(value, current_element)};
          return;
        } else {
          current_element = current_element->right_node.get();
        }
      }
    }
  }
  std::optional<sorted_btree_view<data_type>> find_if(const data_type& value) {
    auto current_element = this;
    while (current_element) {
      if (value < current_element->m_value) {
        current_element = current_element->left_node.get();
      } else if (current_element->m_value < value) {
        current_element = current_element->right_node.get();
      } else {
        return sorted_btree_view<data_type>{current_element};
      }
    }
    return std::nullopt;
  }
  data_type& value() { return this->m_value; }
  const data_type& value() const { return this->m_value; }
  bool has_right() const { return this->right_node.get() != nullptr; }
  bool has_left() const { return this->left_node.get() != nullptr; }
  bool has_parent() const { return this->m_parent != nullptr; }
  sorted_btree_view<data_type> right() { return sorted_btree_view<data_type>{right_node.get()}; }
  sorted_btree_view<data_type> left() { return sorted_btree_view<data_type>{left_node.get()}; }
  sorted_btree_view<data_type> parent() { return sorted_btree_view<data_type>(m_parent); }
  sorted_btree_const_view<data_type> right() const { return sorted_btree_const_view<data_type>{right_node.get()}; }
  sorted_btree_const_view<data_type> left() const { return sorted_btree_const_view<data_type>{left_node.get()}; }
  sorted_btree_const_view<data_type> parent() const { return sorted_btree_const_view<data_type>{m_parent}; }
 private:
  sorted_btree(const data_type& value, sorted_btree* ptr) : m_value{value}, m_parent{ptr} {}
  data_type m_value;
  std::unique_ptr<sorted_btree> left_node = nullptr;
  std::unique_ptr<sorted_btree> right_node = nullptr;
  sorted_btree<data_type>* m_parent = nullptr;
};

template <class data_type>
class sorted_btree_const_view {
 public:
  sorted_btree_const_view() = delete;
  sorted_btree_const_view(const sorted_btree<data_type>* const ptr) : view{ptr} {}
  sorted_btree_const_view(const sorted_btree_const_view&) = default;
  sorted_btree_const_view(sorted_btree_const_view&&) = default;
  sorted_btree_const_view& operator=(const sorted_btree_const_view&) = default;
  sorted_btree_const_view& operator=(sorted_btree_const_view&&) = default;
  ~sorted_btree_const_view() = default;
  const data_type& value() const { return this->view->value(); }
  bool has_right() const { return this->view->has_right(); }
  bool has_left() const { return this->view->has_left(); }
  bool has_parent() const { return this->view->has_parent(); }

  sorted_btree_const_view<data_type> parent() const { return this->view->parent(); }
  sorted_btree_const_view<data_type> right() const { return this->view->right(); }
  sorted_btree_const_view<data_type> left() const { return this->view->left(); }

 private:
  const sorted_btree<data_type>* const view;
};

template <class data_type>
class sorted_btree_view {
 public:
  sorted_btree_view() = delete;
  sorted_btree_view(sorted_btree<data_type>* const ptr) : view{ptr} {}
  sorted_btree_view(const sorted_btree_view&) = default;
  sorted_btree_view(sorted_btree_view&&) = default;
  sorted_btree_view& operator=(const sorted_btree_view&) = default;
  sorted_btree_view& operator=(sorted_btree_view&&) = default;
  ~sorted_btree_view() = default;
  operator sorted_btree_const_view<data_type>() const { return sorted_btree_const_view<data_type>(this->view); }

  data_type& value() { return this->view->value(); }
  const data_type& value() const { return this->view->value(); }
  bool has_right() const { return this->view->has_right(); }
  bool has_left() const { return this->view->has_left(); }
  const sorted_btree_view<data_type> right() const { return this->view->right(); }
  const sorted_btree_view<data_type> left() const { return this->view->left(); }

 private:
  sorted_btree<data_type>* const view;
};

namespace sorted_btree_util {

enum class order {
  PRE_ORDER,
  IN_ORDER,
  POST_ORDER,
};

namespace {
template <class data_type, order when, class func>
void print_view(const sorted_btree_const_view<data_type>& view, func&& formatting) {
  if constexpr (when == order::PRE_ORDER) {
    std::string temp = formatting(view.value());
    std::cout.write(temp.data(), temp.size());
  }
  if (view.has_left()) {
    print_view<data_type, when, func>(view.left(), std::forward<func>(formatting));
  }
  if constexpr (when == order::IN_ORDER) {
    std::string temp = formatting(view.value());
    std::cout.write(temp.data(), temp.size());
  }
  if (view.has_right()) {
    print_view<data_type, when, func>(view.right(), std::forward<func>(formatting));
  }
  if constexpr (when == order::POST_ORDER) {
    std::string temp = formatting(view.value());
    std::cout.write(temp.data(), temp.size());
  }
}
template <class data_type>
std::size_t size_view(const sorted_btree_const_view<data_type>& view) {
  std::set<const sorted_btree_const_view<data_type>*> visited;
  auto* current_element = &view;
  std::size_t result = 0;
  while (current_element) {
    bool left_visited = false;
    bool right_visited = false;
    if (view.has_left()) {
      if (visited.find(&current_element->left()) == visited.end()) {
        current_element = &view.left();
      } else {
        left_visited = true;
      }
    } else {
      left_visited = true;
    }
    if (view.has_right()) {
      if (visited.find(&current_element->right()) == visited.end()) {
        current_element = &view.right();
      } else {
        right_visited = true;
      }
    } else {
      right_visited = true;
    }
    if (right_visited && left_visited) {
      if (current_element->has_parent()) {
        visited.insert(current_element);
        result += 1;
        current_element = &(current_element->parent());
      }
    }
    
  }
  return result;
}
}  // namespace
template <class data_type, order when, class func>
void print(const sorted_btree<data_type>& tree, func&& formatting) {
  print_view<data_type, when, func>(tree, std::forward<func>(formatting));
}

template <class data_type>
std::size_t size(const sorted_btree<data_type>& tree) {
  return size_view<data_type>(tree);
}

}  // namespace sorted_btree_util
}  // namespace container::tree