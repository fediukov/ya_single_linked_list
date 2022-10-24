#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next)
        {
        }

        Node& operator=(const Node& rhs)
        {
            next_node = rhs.next_node;
            return *this;
        }

        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node)
        {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
        {
            node_ = other.node_;
        }

         BasicIterator& operator=(const BasicIterator& rhs) = default;
 
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept
        {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept
        {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept
        {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept
        {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept
        {
            assert(node_ != nullptr);
            this->node_ = this->node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept
        {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] reference operator*() const noexcept
        {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    explicit SingleLinkedList(Type val) noexcept
    {
        head_ = Node(val, nullptr);
        size_ = 0;
    }

    SingleLinkedList(std::initializer_list<Type> values)
    {
        auto it = values.end();
        for (--it; it != values.begin(); --it)
        {
            PushFront(*it);
        }
        PushFront(*it);
    }

    SingleLinkedList(const SingleLinkedList& other)
    {
        for (const auto& item : other)
        {
            PushBack(item);
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs)
    {
        if (this != &rhs) {
            auto tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    ~SingleLinkedList()
    {
        Clear();
    }

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator{ &head_ };
    }

    [[nodiscard]] Iterator end() noexcept
    {
        return Iterator{ nullptr };
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return ConstIterator{ (const_cast<Node*>(&head_)) };
    }

    [[nodiscard]] ConstIterator end() const noexcept
    {
        return ConstIterator{ nullptr };
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator{ (const_cast<Node*>(&head_)) };
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
        return ConstIterator{ nullptr };
    }

    void swap(SingleLinkedList& other) noexcept
    {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }

    [[nodiscard]] size_t GetSize() const noexcept
    {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept
    {
        return size_ ? false : true;
    }

    void PushFront(const Type& value)
    {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PushBack(const Type& value)
    {
        if (IsEmpty())
        {
            head_.next_node = new Node(value, head_.next_node);
        }
        else
        {
            auto current = head_.next_node;
            auto current_two = head_.next_node;
            while (current != nullptr)
            {
                current_two = current;
                current = current->next_node;
            }
            current_two->next_node = new Node(value, nullptr);
        }
        ++size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value)
    {
        assert(pos.node_ != nullptr);
        Node* tmp = new Node(value, pos.node_->next_node);
        pos.node_->next_node = tmp;
        ++size_;
        return { static_cast<Iterator>(tmp) };
    }

    void PopFront() noexcept
    {
        if (head_.next_node != nullptr)
        {
            auto tmp = head_.next_node;
            head_.next_node = (*tmp).next_node;
            delete tmp;
            --size_;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        assert(pos.node_ != nullptr);
        if (pos.node_->next_node != nullptr)
        {
            Node* delete_pos = pos.node_->next_node;
            Node* tmp = pos.node_->next_node->next_node;
            pos.node_->next_node = tmp;
            delete delete_pos;
            --size_;
            return { static_cast<Iterator>(tmp) };
        }
        else
        {
            Node* delete_pos = pos.node_->next_node;
            delete delete_pos;
            --size_;
            return { static_cast<Iterator>(nullptr) };
        }
    }

    void Clear() noexcept
    {
        while (size_)
        {
            PopFront();
        }
    }

    void PrintDetails()
    {
        auto* tmp = head_.next_node;
        while (tmp != nullptr)
        {
            std::cout << tmp << "->" << tmp->next_node << " " << tmp->value << std::endl;
            tmp = tmp->next_node;
        }
    }

private:
    Node head_;
    size_t size_ = 0;
};

// compair functions
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());

}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return rhs < lhs;

}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return lhs < rhs;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return lhs > rhs;
}

// swap
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept
{
    lhs.swap(rhs);
}

// print
void PrintSingleLinkedList(const SingleLinkedList<int>& list)
{
    for (const auto& item : list)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}