#pragma once

#include <string>

namespace pc::rss
{
   namespace
   {
      using Param = std::string;
   } // namespace
   struct Enclosure
   {
      Param type;
      Param url;
      // In Bytes
      std::size_t length;
   };
} // namespace pc::rss