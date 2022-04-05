#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        ///*
        Node& operator=(const Node& rhs) {
            next_node = rhs.next_node;
            //value = rhs.value;
            return *this;
        }
        //*/
        Type value;
        Node* next_node = nullptr;
    };

    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) {
            // assert(false);
            // Реализуйте конструктор самостоятельно
            node_ = node;
        }

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            // assert(false);
            // Реализуйте конструктор самостоятельно
            node_ = other.node_;
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;
        /* {
            node_ = rhs.node_;
        }*/

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            // assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            // assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
            return node_ != rhs.node_;
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            //assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            // assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
            return node_ != rhs.node_;
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            this->node_ = this->node_->next_node;
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            // assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
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

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;


    // constructors
    SingleLinkedList() = default;

    explicit SingleLinkedList(Type val) noexcept {
        head_ = Node(val, nullptr);
        size_ = 0;
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        auto it = values.end();
        for (--it; it != values.begin(); --it)
        {
            PushFront(*it);
        }
        PushFront(*it);
        /*for (const auto& value : values)
        {
            PushBack(value);
        }*/
    }

    SingleLinkedList(const SingleLinkedList& other) {
        for (const auto& item : other)
        {
            PushBack(item);
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            auto tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    // destructors
    ~SingleLinkedList() {
        Clear();
    }

    // something with iterators
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{ (const_cast<Node*>(&head_)) };
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ (const_cast<Node*>(&head_)) };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
    }

    // Swap - Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }

    // GetSize - Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // IsEmpty - Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ ? false : true;
    }

    // PushFront - Вставляет элемент value в начало списка за время O(1)
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    // PushBack
    void PushBack(const Type& value) {
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

    // InsertAfter - Вставляет элемент после итератора pos
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* tmp = new Node(value, pos.node_->next_node);
        pos.node_->next_node = tmp;
        ++size_;
        return { static_cast<Iterator>(tmp) };
    }

    // PopFront
    void PopFront() noexcept {
        if (head_.next_node != nullptr)
        {
            auto tmp = head_.next_node;
            head_.next_node = (*tmp).next_node;
            delete tmp;
            --size_;
        }
    }

    // Erase After
    Iterator EraseAfter(ConstIterator pos) noexcept {
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

    // Clear - Очищает список за время O(N)
    void Clear() noexcept {
        while (size_)
        {
            PopFront();
            /*auto tmp = head_.next_node;
            head_.next_node = (*tmp).next_node;
            delete tmp;
            --size_;*/
        }
    }

    // Print list with details
    void PrintDetails()
    {
        auto* tmp = head_.next_node;
        while (tmp != nullptr)
        {
            std::cout << tmp << "->" << tmp->next_node << " " << tmp->value << std::endl;
            tmp = tmp->next_node;
        }
        if (tmp == nullptr)
        {
            //std::cout << "nullptr" << std::endl;
        }
        else
        {
            std::cout << "Damn it!" << std::endl;
        }
    }

private:
    Node head_;
    size_t size_ = 0;
};

// ==
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (std::equal(lhs.begin(), lhs.end(), rhs.begin()))
    {
        return true;
    }
    return false;
}

// !=
template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs == rhs)
    {
        return false;
    }
    return true;
}

// <
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()))
    {
        return true;
    }
    return false;
}

// >
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (rhs < lhs)
    {
        return true;
    }
    return false;
}

// >=
template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs < rhs)
    {
        return false;
    }
    return true;
}

// <=
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs > rhs)
    {
        return false;
    }
    return true;
}

// swap()
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

// Print
void PrintSingleLinkedList(const SingleLinkedList<int>& list)
{
    for (const auto& item : list)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}