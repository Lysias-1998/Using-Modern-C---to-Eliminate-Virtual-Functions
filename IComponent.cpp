/* Developing Kernel Drivers with Modern C++ - Pavel Yosifovich */

#include <cassert>
#include <iostream>

struct IComponent {
  enum { IID = 0 };

  virtual void QueryInterface(long iid, void **ppv) = 0;
};

struct IFilter : IComponent {
  enum { IID = 10 };
};

struct IProcessNotify : IComponent {
  enum { IID = 11 };
};

template <typename... Interfaces> struct ComponentBase : public Interfaces... {
  static_assert(sizeof...(Interfaces) > 0);

public:
  void QueryInterface(long iid, void **ppv) {
    assert(ppv != nullptr);
    auto p = QueryInterface<Interfaces...>(iid);
    if (p) {
      *ppv = p;
    } else {
      *ppv = nullptr;
    }
  }

private:
  template <typename Interface, typename... Rest>
  void *QueryInterface(long iid) {
    if (iid == Interface::IID) {
      return static_cast<Interface *>(this);
    } else {
      return QueryInterface<Rest...>(iid);
    }
  }

  template <int = 0> void *QueryInterface(long) { return nullptr; }
};

struct MyFilter : public ComponentBase<IFilter, IProcessNotify> {};

int main() {
  IFilter *filter;
  auto c = MyFilter{};
  long iid;
  std::cin >> iid;
  c.QueryInterface(iid, (void **)&filter);
  if (filter) {
    std::cout << "Required interface is supported.\n";
  } else {
    std::cout << "Required interface is not supported.\n";
  }
}