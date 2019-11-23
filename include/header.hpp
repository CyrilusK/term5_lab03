// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <atomic>
#include <iostream>
using namespace std;
class Counter  // счетчик указателей
{
 public:
  atomic_uint count;  // счетчик
};

template <typename T>
class SharedPtr {
 private:
  T* pointer;        // указатель
  Counter* p_count;  // указатель на счетчик
 public:
  SharedPtr()  // конструктор без параметров
  {
    pointer = nullptr;
    p_count = nullptr;
  }

  SharedPtr(T* ptr)  // создание объекта - умного указателя, а параметр -
                     // обычный указатель
  {
    pointer = new T;
    memcpy(pointer, ptr, sizeof(T));
    p_count = new Counter;
    p_count->count = 1;
  }

  SharedPtr(const SharedPtr& r)  // конструктор копирования
  {
    if (r.p_count != nullptr) {
      ++(r.p_count->count);
      p_count = r.p_count;
      pointer = r.pointer;
    } else {
      p_count = nullptr;
    }
  }

  SharedPtr(SharedPtr&& r)  //конструктор перемещения
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

  auto operator=(const SharedPtr& r) -> SharedPtr&  //копирующее присваивание
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

  auto operator=(SharedPtr&& r) -> SharedPtr&  // перемешающее присваивание
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

  // проверяет, указывает ли указатель на объект
  operator bool() const {
    if (pointer != nullptr) {
      return true;
    }
    return false;
  }

  auto operator*() const -> T&  // обращение по адресу
  {
    return *this;
  }
  auto operator-> () const -> T*  // обращение через указатель
  {
    return this;
  }

  auto get() -> T*  // получение указателя на область памяти
  {
    return pointer;
  }
  void reset()  // сброс указателя
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

  void reset(T* ptr)  // сбрасываем указатель и настраиваем на новую память
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

  void Swap(SharedPtr& r)  // меняем указатели местами
  {
    swap(pointer, r.pointer);
    swap(p_count, r.p_count);
  }
  // возвращает количество объектов SharedPtr, которые ссылаются на тот же
  // управляемый объект
  auto use_count() const -> size_t { return p_count->count; }
};
#endif // INCLUDE_HEADER_HPP_
