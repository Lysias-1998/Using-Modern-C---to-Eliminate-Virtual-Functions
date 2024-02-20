#include <concepts>
#include <tuple>
#include <variant>
#include <vector>

template <typename T>
concept CFoo = requires(T foo) {
  { foo.func() } -> std::integral;
};

struct Foo0 {
  auto func() const -> int { return 6; }
};
struct Foo {
  auto func() const -> int { return 7; }
};

Foo foo{};
auto func(CFoo auto &foo2) {}

static_assert(CFoo<Foo>);

// Owning a polymorphic type
template <typename T, typename... Ts>
concept same_as_any = (... or std::same_as<T, Ts>);

template <CFoo... TFoos> class Bar {
public:
  constexpr Bar(same_as_any<TFoos...> auto input_foo) : foo{input_foo} {}

  constexpr auto set_foo(same_as_any<TFoos...> auto input_foo) -> void {
    foo = input_foo;
  }

private:
  std::variant<TFoos...> foo{};
};

Bar<Foo> bar1{Foo{}};
Bar<Foo0, Foo> bar2{Foo{}};

// Storing multiple types
template <CFoo... TFoos> class Baz {
public:
  template <same_as_any<TFoos...> T> auto store(T value) {
    return std::get<std::vector<T>>(data).push_back(value);
  }

private:
  std::tuple<std::vector<TFoos>...> data{};
};

static void store() {
  using foo_storage_t = Baz<Foo0, Foo>;
  foo_storage_t baz{};
  baz.store(Foo0{});
  baz.store(Foo{});
}