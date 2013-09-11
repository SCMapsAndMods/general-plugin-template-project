//Primitive intrusive linked list implementation by pastelmind
//Based on StarCraft's internal code and Patrick Wyatt's article at:
// http://www.codeofhonor.com/blog/avoiding-game-crashes-related-to-linked-lists

#pragma once
#include <cstddef>
#include "../../types.h"


//-------- CLink declaration --------//

template <class T>
class CLink {
  public:
    T *prev;
    T *next;

    /// Returns the object containing this node.
    template <CLink<T> T::*link>
    T* self();

    template <CLink<T> T::*link>
    void insertAfter(T* &head, T* &tail);
};


//Handle for CLists
template <class T, CLink<T> T::*link>
class CListExtern {
  public:
    T* &head;
    T* &tail;
    CListExtern(T **head_, T **tail_) : head(*head_), tail(*tail_) {};
    CListExtern(u32 head_, u32 tail_) : head(*(T**)head_), tail(*(T**)tail_) {};

    void insertAfterHead(T *t) const;
    void unlink(T *t) const;
};

/// Inserts @p t into the linked list.
template <class T, CLink<T> T::*link>
void CListExtern<T, link>::insertAfterHead(T *t) const {
  if (head) {
    if (head == tail)
      tail = t;
    (t->*link).prev = head;
    (t->*link).next = (head->*link).next;
    if ((head->*link).next)
      ((head->*link).next->*link).prev = t;
    (head->*link).next = t;
  }
  else {
    head = t;
    tail = t;
  }
}

/// Unlinks @p t from the linked list.
template <class T, CLink<T> T::*link>
void CListExtern<T, link>::unlink(T *t) const {
  if (head == t)
    head = (t->*link).next;
  if (tail == t)
    tail = (t->*link).prev;

  if ((t->*link).prev)
    ((t->*link).prev->*link).next = (t->*link).next;
  if ((t->*link).next)
    ((t->*link).next->*link).prev = (t->*link).prev;

  (t->*link).prev = NULL;
  (t->*link).next = NULL;
}



template <class T>
template <CLink<T> T::*link>
T* CLink<T>::self() {
  return (T*)((size_t)(this) - offsetof(T, *link));
}

template <class T>
template <CLink<T> T::*link>
void CLink<T>::insertAfter(T* &head, T* &tail) {
  if (head) {
    if (head == tail)
      tail = this->self<link>();
    this->prev = head;
    this->next = (head->*link).next;
    if ((head->*link).next)
      ((head->*link).next->*link).prev = this->self<link>();
    (head->*link).next = this->self<link>();
  }
  else {
    head = this->self<link>();
    tail = this->self<link>();
  }
}
