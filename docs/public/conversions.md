# C++ to python conversions #

This page shows what is automatically translated and to what.
On the left is the `C++` and to the right what the corresponding interface in `javascript` will be.
Not converted means there will be no automatic translation to a corresponding `javascript` object.
Note that any restriction this poses only applies to the public interface of your code (e.g. your public headers).

## Conversion tables ##

| C++                                 | Wasm translation               |
|:----------------------------------- |:------------------------------ |
| Namespace                           | ???                            |
| Nested namespace                    | ???                            |
| Class                               | Class                          |
| Public function                     | Class function                 |
| Private function                    | Not converted                  |
| Static member function              | Static class function          |
| Static member variable              | Static member variable         |
| Public const member variable        | Read only property             |
| Public non const member variable    | Read write property            |
| Private member variable             | Not converted                  |
| Global variable                     | Module variable                |
| Global static variable              | Module variable                |
| Free function                       | Module function                |
| Overloaded function                 | Module function*               |
| Enum                                | Enum                           |
| Scoped enum                         | Enum                           |
| Templated class/function            | Not converted**                |
| Specialized class template          | Class***                       |
| Specialized function template       | Function****                   |

\* Will try to match arguments to types provided.

\*\* No direct translation to `javascript`. Will not emit warning.

\*\*\* The naming convention for these classes can be found under the [Template Naming Convention page](template_naming_convention.md).

\*\*\*\* Functions with different template arguments will behave as overloaded functions.

| C++ Standard library class      | Wasm translation                                                  |
|:------------------------------- |:----------------------------------------------------------------- |
| std::array                      | array                                                             |
| std::complex                    | ???                                                               |
| std::deque                      | ???                                                               |
| std::filesystem::path           | ???                                                               |
| std::forward\_list              | Not converted                                                     |
| std::function                   | ???                                                               |
| std::list                       | ???                                                               |
| std::map                        | Map Object*                                                       |
| std::multimap                   | Not converted                                                     |
| std::multiset                   | Not converted                                                     |
| std::optional                   | ???                                                               |
| std::pair                       | ???                                                               |
| std::priority\_queue            | Not converted                                                     |
| std::queue                      | Not converted                                                     |
| std::set                        | ???                                                               |
| std::shared_ptr                 | ???                                                               |
| std::stack                      | Not converted                                                     |
| std::tuple                      | ???                                                               |
| std::unique_ptr                 | ???                                                               |
| std::shared_ptr                 | ???                                                               |
| std::unordered\_map             | ???                                                               |
| std::unordered\_multimap        | Not converted                                                     |
| std::unordered\_multiset        | Not converted                                                     |
| std::unordered\_set             | ???                                                               |
| std::valarray                   | ???                                                               |
| std::variant                    | ???                                                               |
| std::vector                     | Vector Object**                                                   |

\* Converted via the [`register_map`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#built-in-type-conversions) function (behaves like a `Object` in `javascript`).

\*\* Converted via the [`register_vector`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#built-in-type-conversions) function (behaves like a `Object` in `javascript`).


| C++ builtin type           | Wasm translation                                                 |
|:-------------------------- |:---------------------------------------------------------------- |
| bool                       | true || false                                                    |
| char                       | Number                                                           |
| char16\_t                  | Number                                                           |
| char32\_t                  | Number                                                           |
| double                     | Number                                                           |
| float                      | Number                                                           |
| int                        | Number                                                           |
| int8_t                     | Number                                                           |
| int16_t                    | Number                                                           |
| int32_t                    | Number                                                           |
| int64_t                    | Number                                                           |
| long double                | Number                                                           |
| long int                   | Number                                                           |
| long long int              | Number                                                           |
| short int                  | Number                                                           |
| signed char                | Number                                                           |
| string                     | ArrayBuffer, Uint8Array, Uint8ClampedArray, Int8Array, or String |
| string\_view               | ???                                                              |
| uint8_t                    | Number                                                           |
| uint16_t                   | Number                                                           |
| uint32_t                   | Number                                                           |
| uint64_t                   | Number                                                           |
| unsigned char              | Number                                                           |
| unsigned int               | Number                                                           |
| unsigned long int          | Number                                                           |
| unsigned long long int     | Number                                                           |
| unsigned short int         | Number                                                           |
| wchar\_t                   | Number                                                           |

