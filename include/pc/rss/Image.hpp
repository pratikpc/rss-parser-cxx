#pragma once

#include <optional>
#include <string>

namespace pc::rss
{
   namespace
   {
      using Param    = std::string;
      using OptParam = std::optional<Param>;
   } // namespace
   struct Image
   {
      Param    title;
      Param    link;
      Param    url;
      OptParam width;
      OptParam height;
      OptParam description;
   };
} // namespace pc::rss