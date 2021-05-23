#pragma once

#include <string>

namespace pc::rss
{
   namespace
   {
      using Param = std::string;
      using Bytes = std::size_t;
   } // namespace
   struct Enclosure
   {
      Param type;
      Param url;
      Bytes length = 0;
   };
} // namespace pc::rss