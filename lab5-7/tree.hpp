#pragma once

class tree_elem
{
 public:
     int m_data;
     tree_elem * m_left;
     tree_elem * m_right;
     tree_elem(int val)
     {
         m_left = nullptr;
         m_right = nullptr;
         m_data = val;
     }
};

class binary_tree
{
 private:
    tree_elem * m_root;
    int m_size;
    void print_tree(tree_elem *);
    void delete_tree(tree_elem *);

 public:
    binary_tree(int);
    ~binary_tree();
    void print();
    bool find(int);
    void insert(int);
    void erase(int);
    int parent(int);
    int size();
};

