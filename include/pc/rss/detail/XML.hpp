#pragma once

#if __has_include("generator")
#   include <generator>
#   define PC_RSS_PARSER_USE_STD_GENERATOR
#   define PC_RSS_ENABLE_GENERATOR_FUNCTIONS
#elif __has_include("experimental/generator")
#   include <experimental/generator>
#   define PC_RSS_PARSER_USE_EXPERIMENTAL_GENERATOR
#   define PC_RSS_ENABLE_GENERATOR_FUNCTIONS
#endif
#include <optional>
#include <string>
#include <sstream>

#include <pugixml.hpp>

namespace pc::rss
{
   namespace
   {
      using Param    = std::string;
      using OptParam = std::optional<Param>;
      using IntParam = std::size_t;

      namespace detail
      {
#ifdef PC_RSS_PARSER_USE_STD_GENERATOR
         using std::generator;
#endif
#ifdef PC_RSS_PARSER_USE_EXPERIMENTAL_GENERATOR
         using std::experimental::generator;
#endif
      } // namespace detail
   }    // namespace
} // namespace pc::rss
