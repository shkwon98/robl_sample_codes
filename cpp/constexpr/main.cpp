#include <iostream>
#include <string>
#include <string_view>

// 1순위
// 아래 세 타입은 동일하다. 컴파일 시점에 값이 결정되며, 수정할 수 없는 문자열.
constexpr std::string_view my_string_13 = "hello, world!";
constexpr const std::string_view my_string_9 = "hello, world!";
const constexpr std::string_view my_string_11 = "hello, world!";

// 2순위
// 런타임에 값이 결정되며, 수정할 수 없는 문자열.
const std::string_view my_string_15 = "hello, world!";

// 3순위
// 아래 두 타입은 동일하다. 컴파일 타임에 값이 결정되며, 수정할 수 없는 문자열.
constexpr const std::string_view &my_string_10 = "hello, world!";
const constexpr std::string_view &my_string_12 = "hello, world!";

// 4순위
// 수정할 수 없는 문자열의 레퍼런스.
const std::string_view &my_string_16 = "hello, world!";

// 5순위
// 런타임에 값이 결정되며, 수정할 수 없는 문자열.
const std::string my_string_7 = "hello, world!";
// 수정할 수 없는 문자열의 레퍼런스.
const std::string &my_string_8 = "hello, world!";
