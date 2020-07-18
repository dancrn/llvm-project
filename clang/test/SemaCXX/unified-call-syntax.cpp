// RUN: %clang_cc1 -std=c++20 -fsyntax-only -pedantic -fufcs -verify -DPEDANTIC %s
struct Type {
  int g;
  int method(const char *z) const; // expected-note {{candidate function}}
};

int method(this const Type &a, int b); // expected-note {{candidate function}}

// basic principles of ufcs
int func() {
  // these two calls are exactly equivalent
  int a1 = Type().method(2) + method(Type(), 2);

  // and the ufcs candidate doesn't otherwise mask a class' original methods
  int a2 = Type().method("x");

  return 0;
}

namespace n1 {
  template<typename T>
  int method(this const T &t, double x) { // expected-note {{candidate function [with T = SubType]}}
    static_assert(false, "used"); // expected-error {{static_assert failed "used"}}
  }

  // normal overload resolution rules apply
  int f2() {
    return Type().method(3.0);
  }
}

template<typename T> requires (false) //expected-note {{because 'false' evaluated to false}}
int bar(this const T &t, int x); // expected-note {{candidate template ignored: constraints not satisfied [with T = Type]}}

template<typename T>
int bar(this T &t, int x); // expected-note {{candidate function [with T = Type] not viable: expects an l-value for 1st argument}};

int f3() {
  return Type().bar(1); // expected-error {{no matching member function for call to 'bar'}}
}

int method(this const Type &x, const char *z); // expected-note {{candidate function}}

int f4() {
  return Type().method("a"); // expected-error {{call to member function 'method' is ambiguous}}
}

int baz(const Type &t, int x);

// ufcs must not consider non-ufcs-qualified functions
int f5() {
  return Type().baz(1); // expected-error {{no member named 'baz' in 'Type'}}
}

struct SubType: public Type {
  int h;
};

// SubType is castable to Type, so ufcs candidacy still applies
int f6() {
  return SubType().method(2);
}

// ufcs should work with namespaces
int f7() {
  using namespace n1;
  return SubType().method(1.0);
}

// .. which can introduce ambiguity
int f8() {
  using namespace n1;
  return SubType().method(1); // expected-error {{call to member function 'method' is ambiguous}}
}
