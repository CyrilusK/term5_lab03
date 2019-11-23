// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <atomic>
#include <iostream>
using namespace std;
class Counter  // ������� ����������
{
 public:
  atomic_uint count;  // �������
};

template <typename T>
class SharedPtr {
 private:
  T* pointer;        // ���������
  Counter* p_count;  // ��������� �� �������
 public:
  SharedPtr()  // ����������� ��� ����������
  {
    pointer = nullptr;
    p_count = nullptr;
  }

  SharedPtr(T* ptr)  // �������� ������� - ������ ���������, � �������� -
                     // ������� ���������
  {
    pointer = new T;
    memcpy(pointer, ptr, sizeof(T));
    p_count = new Counter;
    p_count->count = 1;
  }

  SharedPtr(const SharedPtr& r)  // ����������� �����������
  {
    if (r.p_count != nullptr) {
      ++(r.p_count->count);
      p_count = r.p_count;
      pointer = r.pointer;
    } else {
      p_count = nullptr;
    }
  }

  SharedPtr(SharedPtr&& r)  //����������� �����������
  {
    pointer = r.pointer;
    p_count = r.p_count;
    r.pointer = nullptr;
    r.p_count = nullptr;
  }

  ~SharedPtr() {
    if (p_count->count > 0) {
      --(p_count->count);
      if (p_count->count == 0) {
        delete p_count;
        delete pointer;
      }
    }
  }

  auto operator=(const SharedPtr& r) -> SharedPtr&  //���������� ������������
  {
    if (pointer == nullptr) {
      if (r.pointer != nullptr) {
        ++(r.p_count->count);
        pointer = r.pointer;
        p_count = r.p_count;
      }
    } else {
      if (r.pointer != pointer) {
        if (r.pointer != nullptr) {
          --(p_count->count);
          if (p_count->count == 0) {
            delete p_count;
            delete pointer;
          }
          ++(r.p_count->count);
          pointer = r.pointer;
          p_count = r.p_count;
        } else {
          --(p_count->count);
          if (p_count->count == 0) {
            delete p_count;
            delete pointer;
          }
          p_count = nullptr;
          pointer = nullptr;
        }
      }
    }
    return *this;
  }

  auto operator=(SharedPtr&& r) -> SharedPtr&  // ������������ ������������
  {
    if (pointer == nullptr) {
      if (r.pointer != nullptr) {
        pointer = r.pointer;
        p_count = r.p_count;
        r.pointer = nullptr;
        r.p_count = nullptr;
      }
    } else {
      if (r.pointer != pointer) {
        if (r.pointer != nullptr) {
          --(p_count->count);
          if (p_count->count == 0) {
            delete p_count;
            delete pointer;
          }
          pointer = r.pointer;
          p_count = r.p_count;
        } else {
          --(p_count->count);
          if (p_count->count == 0) {
            delete p_count;
            delete pointer;
          }
          pointer = nullptr;
          p_count = nullptr;
        }
      }
    }
    return *this;
  }

  // ���������, ��������� �� ��������� �� ������
  operator bool() const {
    if (pointer != nullptr) {
      return true;
    }
    return false;
  }

  auto operator*() const -> T&  // ��������� �� ������
  {
    return *this;
  }
  auto operator-> () const -> T*  // ��������� ����� ���������
  {
    return this;
  }

  auto get() -> T*  // ��������� ��������� �� ������� ������
  {
    return pointer;
  }
  void reset()  // ����� ���������
  {
    if (p_count != nullptr) {
      --(p_count->count);
      if (p_count->count == 0) {
        delete p_count;
        delete pointer;
      }
      p_count = nullptr;
      pointer = nullptr;
    }
  }

  void reset(T* ptr)  // ���������� ��������� � ����������� �� ����� ������
  {
    if (p_count != nullptr) {
      --(p_count->count);
      if (p_count->count == 0) {
        delete p_count;
        delete pointer;
      }
    }
    if (pointer == nullptr) {
      pointer = new T;
    }
    memcpy(pointer, ptr, sizeof(T));
    if (p_count == nullptr) {
      p_count = new Counter;
    }
    p_count->count = 1;
  }

  void Swap(SharedPtr& r)  // ������ ��������� �������
  {
    swap(pointer, r.pointer);
    swap(p_count, r.p_count);
  }
  // ���������� ���������� �������� SharedPtr, ������� ��������� �� ��� ��
  // ����������� ������
  auto use_count() const -> size_t { return p_count->count; }
};
#endif // INCLUDE_HEADER_HPP_
