// RUN: %clang_cc1 -fsyntax-only -pedantic -fufcs -verify -DPEDANTIC %s

int f1(this double t);

int f2(double x, this double y); // expected-error {{'this' qualifier must qualify the first function parameter}}

int f3(const this double x); // expected-error {{'this' qualifier must precede declaration specifiers}}

struct s1 {
  int f1(this int k); // expected-error {{Unified call syntax only applies to regular functions}}
};

class s2 {
  template<typename T>
  int f1(this T x); // expected-error {{Unified call syntax only applies to regular functions}}
};

class s3 {
  template<typename T>
  int f1(this int k, T x); // expected-error {{Unified call syntax only applies to regular functions}}
};
