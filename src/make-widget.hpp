#pragma once

#include <QLayout>
#include <QWidget>
#include <initializer_list>

class QBoxLayout;

/// You may pass this to an element to ab::makeLayout to insert a stretch
/// item.
///
/// Check ab::addStretch to add support for another layout type.
QObject *stretch();

/// You may pass this to an element to ab::makeLayout to insert a space
/// item.
///
/// Check ab::addStretch to add support for another layout type.
QObject *space();

/// This function is used to add support for passing ab::stretch() as a
/// parameter to ab::makeLayout().
///
/// Create a `void addStretch(YourType *)` to add support.
void addStretch(void *);
void addStretch(QBoxLayout *box);

class Space : public QWidget {
  Q_OBJECT
};

// function_traits is stolen from here:
// https://stackoverflow.com/questions/7943525/is-it-possible-to-figure-out-the-parameter-type-and-return-type-of-a-lambda/7943765

template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())> {};
// For generic types, directly use the result of the signature of its
// 'operator()'

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const>
// we specialize for pointers to member function
{
  enum { arity = sizeof...(Args) };
  // arity is the number of arguments.

  typedef ReturnType result_type;

  template <size_t i> struct arg {
    typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    // the i-th argument is equivalent to the i-th tuple element of a tuple
    // composed of those arguments.
  };
};

/// Creates a new T(args...) with T = first argument of Func, and appies the
/// function UnaryWith on it.
///
/// This allows for a "declarative" style of defining widgets/objects.
template <typename UnaryWith, typename... Args>
auto make(UnaryWith func, Args... args) {
  using FuncArg1 = typename function_traits<UnaryWith>::template arg<0>::type;
  using T = typename std::remove_pointer<FuncArg1>::type;

  auto a = new T(std::forward<Args...>(args)...);
  func(a);
  return a;
}

/// Creates a widget of type T, and applies the function UnaryWith on it.
///
/// This allows for a "declarative" style of defining layouts and widget
/// children.

template <typename T, typename UnaryWith, typename... TArgs>
T *makeWidget(UnaryWith with, TArgs... args) {
  auto t = new T(std::forward<TArgs...>(args)...);

  with(t);

  return t;
}

/// Creates a layout of type T, and adds all elements in widgets.
///
/// @param widgets May contain ab::stretch() to add a stretch item.
template <typename T> T *makeLayout(std::initializer_list<QObject *> widgets) {
  auto t = new T;

  for (auto &&widget : widgets) {
    if (widget == nullptr)
      continue;
    else if (widget == stretch())
      addStretch(t);
    else if (widget == space())
      t->addWidget(new Space());
    else if (widget->isWidgetType())
      t->addWidget(static_cast<QWidget *>(widget));
    else if (auto layout = dynamic_cast<QLayout *>(widget))
      t->addLayout(layout);
    else
      assert(false);
  }

  t->setContentsMargins(0, 0, 0, 0);

  return t;
}

/// Creates a layout of type T, applies the function UnaryWith, and adds all
/// elements in widgets.
///
/// This allows for a "declarative" style of defining layouts and widget
/// children.
///
/// @param widgets May contain ab::stretch() to add a stretch item.
template <typename T, typename UnaryWith>
T *makeLayout(UnaryWith with, std::initializer_list<QObject *> widgets) {
  auto t = makeLayout<T>(widgets);

  with(t);

  return t;
}
