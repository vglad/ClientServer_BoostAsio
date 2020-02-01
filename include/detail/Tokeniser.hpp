#ifndef CLIENTSERVER_INCLUDE_DETAIL_TOKENISER_HPP
#define CLIENTSERVER_INCLUDE_DETAIL_TOKENISER_HPP

#include <sstream>
#include <algorithm>
#include <memory>

namespace clientServer::detail {

//*****************************************************************************
// This is general tokenizer
// Returns only containers of <std::string>
// !!!Do not work if returns containers of numeric type
  template <typename ContainerT>
  ContainerT tokenize_by_delim(std::string const& str,
                               std::string const& delimiters = " ",
                               bool               trimEmpty = false) {
    std::string::size_type pos, lastPos = 0, length = str.length();

    ContainerT tokens;
    using value_type = typename ContainerT::value_type;
    using size_type  = typename ContainerT::size_type;

    while(lastPos < length + 1) {
      pos = str.find_first_of(delimiters, lastPos);
      if(pos == std::string::npos) pos = length;
      if(pos != lastPos || !trimEmpty) {
        tokens.emplace_back(
            value_type(str.data() + lastPos,
                       (size_type)pos - lastPos)
        );
      }
      lastPos = pos + 1;
    }
    return tokens;
  }
//*****************************************************************************

//*****************************************************************************
// Erases non-numeric values from container (\n,\t,\r, \t\t, " "...)
  template<typename ContainerT>
  ContainerT erase_empty_non_num_values(std::unique_ptr<ContainerT> pCont) {
    ContainerT & v = *pCont;
    auto remove_empty =
             [](std::string const& s) {
               try {
                 std::stold(s);
                 return false;
               }
               catch (...) {
                 return true;
               }
             };
    v.erase(std::remove_if(begin(v), end(v), remove_empty), end(v));
    return v;
  }
//*****************************************************************************

//*****************************************************************************
// Creates new container of num type from container of strings
  template<typename ContainerIn, typename ContainerOut>
  ContainerOut str_to_num(std::unique_ptr<ContainerIn> pContIn) {
    ContainerIn & vIn = *pContIn;
    ContainerOut vOut;
    auto to_num = [&vOut](std::string const& s){ vOut.emplace_back(std::stoi(s)); };
    std::for_each(begin(vIn), end(vIn), to_num);
    return vOut;
  }
//*****************************************************************************

//*****************************************************************************
// This tokenizer works only with strings, delimited by whitespace
// Can return container of any type
  template <typename T>
  std::vector<T> tokenize_by_whitespace(std::string const& s) {
    std::istringstream iss(s);
    return {std::istream_iterator<T>{iss}, std::istream_iterator<T>{}};
  }
//*****************************************************************************

}

#endif //CLIENTSERVER_INCLUDE_DETAIL_TOKENISER_HPP
