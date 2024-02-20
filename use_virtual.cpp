#include <memory>
#include <vector>

// Binding interfaces

struct FooInterface0 {
  virtual auto func() const -> int = 0;
};

struct Foo0 : public FooInterface0 {
  auto func() const -> int override { return 7; }
};

struct FooInterface {
  FooInterface() = default;
  FooInterface(const FooInterface &) = default;
  FooInterface(FooInterface &&) = default;
  FooInterface &operator=(const FooInterface &) = default;
  FooInterface &operator=(FooInterface &&) = default;
  virtual ~FooInterface() = default;
};

struct Foo : public FooInterface {
  Foo() = default;
  Foo(const Foo &) = default;
  Foo(Foo &&) = default;
  Foo &operator=(const Foo &) = default;
  Foo &operator=(Foo &&) = default;
  virtual ~Foo() = default;
};

std::unique_ptr<FooInterface> foo = std::make_unique<Foo>();
auto func(std::unique_ptr<FooInterface> foo2) {}

// Owning a polymorphic type
class Bar {
public:
  Bar(std::unique_ptr<FooInterface> input_foo) : foo{std::move(input_foo)} {}

  auto set_foo(std::unique_ptr<FooInterface> input_foo) {
    foo = std::move(input_foo);
  }

private:
  std::unique_ptr<FooInterface> foo{};
};

Bar bar{std::make_unique<Foo>()};

// Storing multiple types
class Baz {
public:
  auto store(std::unique_ptr<FooInterface> value) -> void {
    data.push_back(std::move(value));
  }

private:
  std::vector<std::unique_ptr<FooInterface>> data{};
};

static void store() {
  Baz bazz{};
  bazz.store(std::make_unique<Foo>());
}