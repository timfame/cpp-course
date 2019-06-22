//
// Created by timfame on 22.06.19.
//

#ifndef EXAM_LIST_LIST_H
#define EXAM_LIST_LIST_H

#include <iterator>
#include <cassert>

template <class T>
struct list {
private:

    struct BaseNode {
        BaseNode* left;
        BaseNode* right;

        BaseNode() : left(nullptr), right(nullptr) {}

        BaseNode(BaseNode* l, BaseNode* r) : left(l), right(r) {}

        //    BaseNode(BaseNode* other) : left(other->left), right(other->right) {}
    };

    struct Node : BaseNode {
        T value;

        Node(BaseNode* l, BaseNode* r, T _value) : BaseNode(l, r), value(_value) {}

        //Node(Node* other) : BaseNode(other->left, other->right), value(other->value) {}

        ~Node() = default;
    };

    BaseNode _back_pointer;
    BaseNode* _back;

public:

    template <class L>
    struct LIterator : std::iterator<std::bidirectional_iterator_tag, L> {
        friend list;
        friend BaseNode;

        LIterator() : ptr(nullptr) {}

        ~LIterator() = default;

        template <class Another>
        LIterator(Another* p) : ptr(p) {}

        template <class Another>
        LIterator(LIterator<Another> const& other) : ptr(other.ptr) {}

        template <class Another>
        LIterator& operator=(LIterator<Another> const& other) {
            ptr = other.ptr;
            return *this;
        }

        template <class Another>
        bool operator==(LIterator<Another> const& b) const {
            return ptr == b.ptr;
        }

        template <class Another>
        bool operator!=(LIterator<Another> const& b) const {
            return ptr != b.ptr;
        }

        L& operator*() const {
            return (static_cast<Node *>(const_cast<BaseNode*>(ptr))->value);
        }

        L& operator*() {
            return (static_cast<Node *>(const_cast<BaseNode*>(ptr))->value);
        }

        L* operator->() const {
            return &(static_cast<Node *>(const_cast<BaseNode*>(ptr))->value);
        }

        L* operator->() {
            return &(static_cast<Node *>(const_cast<BaseNode*>(ptr))->value);
        }

        LIterator& operator++() {
            ptr = ptr->right;
            return *this;
        }

        LIterator operator++(int) {
            LIterator tmp(*this);
            ptr = ptr->right;
            return tmp;
        }

        LIterator& operator--() {
            ptr = ptr->left;
            return *this;
        }

        LIterator operator--(int) {
            LIterator tmp(*this);
            ptr = ptr->left;
            return tmp;
        }

    private:
        BaseNode* ptr;
    };


    typedef BaseNode value_type;
    typedef LIterator<T> iterator;
    typedef LIterator<T const> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    list() noexcept;
    list(list const& other);
    list& operator=(list const& other);
    ~list();

    void clear() noexcept;
    bool empty() const noexcept;

    void push_back(T const& element);
    void pop_back() noexcept;
    T& back();
    T const& back() const;

    void push_front(T const& element);
    void pop_front() noexcept;
    T& front();
    T const& front() const;

    friend void swap(list& a, list& b) noexcept {
        std::swap(a._back_pointer, b._back_pointer);
        if (a._back->right == b._back) {
            a._back->left = &a._back_pointer;
            a._back->right = &a._back_pointer;
        }
        if (b._back->right == a._back) {
            b._back->left = &b._back_pointer;
            b._back->right = &b._back_pointer;
        }
        a._back->left->right = &a._back_pointer;
        a._back->right->left = &a._back_pointer;
        b._back->left->right = &b._back_pointer;
        b._back->right->left = &b._back_pointer;
    }

    iterator begin() noexcept {
        return iterator(_back->right);
    }

    iterator end() noexcept  {
        return iterator(_back);
    }

    const_iterator begin() const noexcept {
        return const_iterator(_back->right);
    }

    const_iterator end() const noexcept {
        return const_iterator(_back);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    iterator insert(const_iterator pos, T const& val) {
        BaseNode* ptr = pos.ptr;
        auto new_node = new Node(ptr->left, ptr, val);
        ptr->left->right = new_node;
        ptr->left = new_node;
        return iterator(new_node);
    }

    iterator erase(const_iterator pos) {
        BaseNode* ptr = pos.ptr;
        if (ptr == _back) {
            throw std::runtime_error("Out of bound erase");
        }
        ptr->left->right = ptr->right;
        ptr->right->left = ptr->left;
        BaseNode* ret = ptr->right;
        delete (static_cast<Node *>(ptr));
        return iterator(ret);
    }

    iterator erase(const_iterator first, const_iterator last) {
        BaseNode* left = first.ptr;
        BaseNode* right = last.ptr;
        left = left->left;
        auto next_it = first;
        for (auto it = first; it != last;) {
            ++next_it;
            delete (static_cast<Node *>(it.ptr));
            it = next_it;
        }
        left->right = right;
        right->left = left;
        return iterator(right);
    }

    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last) {
        if (first == last)
            return;
        BaseNode* p1 = pos.ptr->left;
        BaseNode* p2 = pos.ptr;
        BaseNode* i1 = first.ptr;
        BaseNode* i2 = last.ptr->left;
        BaseNode* other_p1 = i1->left;
        BaseNode* other_p2 = last.ptr;
        p1->right = i1;
        i1->left = p1;
        i2->right = p2;
        p2->left = i2;
        other_p1->right = other_p2; // delete from other
        other_p2->left = other_p1;
    }

};

template <class T>
list<T>::list() noexcept : _back_pointer(), _back(&_back_pointer) {
    _back_pointer.left = _back;
    _back_pointer.right = _back;
}

template <class T>
list<T>::list(list const &other) : list() {
    auto tmp = other._back->right;
    while (tmp != other._back) {
        try {
            push_back(static_cast<Node *>(tmp)->value);
        } catch (...) {
            clear();
        }
        tmp = tmp->right;
    }
}

template <class T>
list<T>& list<T>::operator=(list const &other) {
    if (this == &other)
        return *this;
    list<T> buff;
    auto tmp = other._back->right;
    while (tmp != other._back) {
        buff.push_back(static_cast<Node *>(tmp)->value);
        tmp = tmp->right;
    }
    clear();
    swap(*this, buff);
    return *this;
}

template <class T>
list<T>::~list() {
    clear();
}

template <class T>
void list<T>::clear() noexcept {
    while (!empty())
        pop_back();
}

template <class T>
bool list<T>::empty() const noexcept {
    return _back->right == _back;
}

template <class T>
void list<T>::push_back(const T &element) {
  insert(end(), element);
}

template <class T>
void list<T>::pop_back() noexcept {
    iterator it = end();
    --it;
    erase(it);
}

template <class T>
T& list<T>::back() {
    assert(!empty());
    return static_cast<Node *>(_back->left)->value;
}

template <class T>
T const& list<T>::back() const {
    assert(!empty());
    return static_cast<Node *>(_back->left)->value;
}

template <class T>
void list<T>::push_front(const T &element) {
    insert(begin(), element);
}

template <class T>
void list<T>::pop_front() noexcept {
    erase(begin());
}

template <class T>
T& list<T>::front() {
    assert(!empty());
    return static_cast<Node *>(_back->right)->value;
}

template <class T>
T const& list<T>::front() const {
    assert(!empty());
    return static_cast<Node *>(_back->right)->value;
}

#endif //EXAM_LIST_LIST_H
