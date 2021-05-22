#pragma once

#include <optional>
#include <string>

#include <pc/rss/Enclosure.hpp>
#include <pc/rss/Image.hpp>

namespace pc::rss
{
   namespace
   {
      using Param       = std::string;
      using OptParam    = std::optional<Param>;
      using OptImage    = std::optional<rss::Image>;
      using OptIntParam = std::optional<std::size_t>;
   } // namespace

   struct Channel
   {
      Param title;
      Param link;
      Param description;

      OptParam    language;
      OptParam    copyright;
      OptParam    managingEditor;
      OptParam    webMaster;
      OptParam    pubDate;
      OptParam    generator;
      OptParam    docs;
      OptIntParam ttl;
      OptImage    image;
   };
} // namespace pc::rss