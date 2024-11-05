# BadgeCpp

![BadgeCpp](badgecpp.svg)

BadgeCpp是一个使用C++编写的Badge构建系统, 用于生成svg格式的Badge



## 许可协议

本项目采用 [MIT 许可协议](LICENSE)。使用本项目时，请尽可能保留原作者的版权声明，并包含以下链接：
- 项目主页: [https://github.com/MineYuanlu/badgecpp](https://github.com/MineYuanlu/badgecpp)


## 参考代码
> 本项目参考了以下项目的代码

- badge-maker: [shields](https://github.com/badges/shields)
- icons: [simple-icons](https://github.com/simple-icons/simple-icons)
- font: [anafanafo](https://github.com/metabolize/anafanafo)

## 使用方法

1. 将本项目作为子模块添加到你的项目中:
```shell
git submodule add https://github.com/MineYuanlu/badgecpp
```
2. 在你的CmakeLists.txt中添加以下内容:
```cmake
add_subdirectory(badgecpp)
target_link_libraries(your_target badgecpp)
```
3. 在你的源文件中包含头文件:
```cpp
#include "badgecpp/badge.hpp"
```
4. 创建Badge对象并设置属性:
```cpp
badge::Badge svg{
/*         .label */ "label",
/*   .label_color */ std::nullopt,
/*       .message */ "message",
/* .message_color */ std::nullopt,
/*         .style */ style,
/*          .logo */ nullptr,
/*    .logo_color */ std::nullopt,
/*    .logo_width */ std::nullopt,
/*     .id_suffix */ "id",
};
```
5. 生成Badge:
```cpp
badge::Xml xml = svg.makeBadgeXml();
```
```cpp
std::string str = svg.makeBadge(); // or str = xml.render();
```
## 示例

1. [main.cpp](main.cpp)
2.
```cpp
badge::Badge svg{
    "label",
    badge::Color("#fff"),
    "message",
    badge::Color("red"),
    style,
    badge::BuiltinIcons::by_title("C++"),
    badge::Color("hsla(200, 50%, 50%, 0.5)"),
    17,
    "id",
};
```