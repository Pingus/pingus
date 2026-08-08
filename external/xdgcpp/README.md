# xdg [![Build Status](https://travis-ci.org/vosst/xdg.svg?branch=master)](https://travis-ci.org/vosst/xdg)

A straightforward implementation of the XDG Base Directory Specification in C++11.
I became tired of retyping and retesting the same functionality over and over again,
so I decided to place my own little helper here.

## Dependencies

 - boost::filesystem: For handling all things filesystem paths.
 - boost::system:: Required by boost::filesystem.
 - boost::test: For testing purposes obviously.

Install with
```bash
sudo apt-get install libboost-filesystem-dev libboost-system-dev libboost-test-dev
```

## Quick'n'Easy Integration

xdg provides free functions that are easy to integrate with existing projects.
```cpp
#include <xdg.h>

#include <iostream>

int main()
{
	std::cout << xdg::data().home() << std::endl;
	std::cout << xdg::config().home() << std::endl;
	std::cout << xdg::cache().home() << std::endl;
	std::cout << xdg::runtime().dir() << std::endl;

	return 0;
}
```

## Complete Integration

The interface xdg::BaseDirSpecification can be used to integrate xdg base directory
queries into a code base such that interaction with the xdg::BaseDirSpecification is testable.

```cpp
#include <xdg.h>

class MyClass
{
public:
    MyClass(const std::shared_ptr<xdg::BaseDirSpecification>& bds) : bds{bds}
    {
    }

    void do_something()
    {
        // Query the user-specific config directory.
        auto path = bds->config().home();
        // Do something with the config files.
    }
private:
	std::shared_ptr<xdg::BaseDirSpecification> bds;
};

// In the testing setup, under the assumption of Google Test and Google Mock.
namespace
{
struct MockConfig : public xdg::Config
{
    // ...
};
struct MockBaseDirSpecification : public xdg::BaseDirSpecification
{
    // ...
};
}

TEST(MyClass, do_something_queries_config_home_directory)
{
    using namespace ::testing;
    auto config = std::make_shared<NiceMock<MockConfig>>();
    EXPECT_CALL(*config, home()).Times(1).ReturnRepeatedly("/tmp");

    auto bds = std::make_shared<NiceMock<MockBaseDirSpecification>>();
    ON_CALL(*bds, config()).WillByDefault(ReturnRef(*config));

    MyClass mc{bds};
    mc.do_something();
}
```
