#pragma once

#include <string>

namespace pc::rss
{
   namespace
   {
      using Param = std::string;
   } // namespace
   struct Image
   {
      Param title;
      Param link;
      Param url;
   };
} // namespace pc::rss