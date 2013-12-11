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
    CListExtern(T **head, T **tail) : head(*head), tail(*tail) {};
    CListExtern(T* &head, T* &tail) : head(head), tail(tail) {};
    CListExtern(u32 head, u32 tail) : head(*(T**)head), tail(*(T**)tail) {};

    /// Inserts @p t into the linked list, before the @p target.
    void insertBefore(T *t, T *target) const;

    /// Inserts @p t into the linked list, before the list head.
    void insertBeforeHead(T *t) const;

    /// Inserts @p t into the linked list, after the @p target.
    void insertAfter(T *t, T *target) const;

    /// Inserts @p t into the linked list, after the list head.
    void insertAfterHead(T *t) const;


    /// Unlinks @p t from the linked list.
    void unlink(T *t) const;

    /// Unlinks and returns the head element from the linked list.
    T* popHead() const;
};

template <class T, CLink<T> T::*link>
void CListExtern<T, link>::insertBefore(T *t, T *target) const {
  if (head) {
    if (head == target)
      head = t;
    (t->*link).prev = (target->*link).prev;
    (t->*link).next = target;
    if ((target->*link).prev)
      ((target->*link).prev->*link).next = t;
    (target->*link).prev = t;
  }
  else {
    head = t;
    tail = t;
  }
}

template <class T, CLink<T> T::*link>
void CListExtern<T, link>::insertBeforeHead(T *t) const {
  insertBefore(t, head);
}

template <class T, CLink<T> T::*link>
void CListExtern<T, link>::insertAfter(T *t, T *target) const {
  if (head) {
    if (tail == target)
      tail = t;
    (t->*link).prev = target;
    (t->*link).next = (target->*link).next;
    if ((target->*link).next)
      ((target->*link).next->*link).prev = t;
    (target->*link).next = t;
  }
  else {
    head = t;
    tail = t;
  }
}

template <class T, CLink<T> T::*link>
void CListExtern<T, link>::insertAfterHead(T *t) const {
  insertAfter(t, head);
}

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

//Based on function @ 0x004D4E30
template <class T, CLink<T> T::*link>
T* CListExtern<T, link>::popHead() const {
  T *t = head;
  unlink(t);
  return t;
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
