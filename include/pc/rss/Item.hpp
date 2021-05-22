#pragma once

#include <optional>
#include <string>
#include <vector>

#include <pc/rss/Enclosure.hpp>

namespace pc::rss
{
   namespace
   {
      using Param        = std::string;
      using OptParam     = std::optional<Param>;
      using MultiParam   = std::vector<Param>;
      using OptEnclosure = std::optional<Enclosure>;
   } // namespace

   struct Item
   {
      OptParam   title;
      OptParam   link;
      OptParam   description;
      OptParam   author;
      MultiParam category;
      OptParam   comments;
      OptParam   guid;
      OptParam   pubDate;
      OptParam   source;

      OptEnclosure enclosure;
   };
} // namespace pc::rss
