#pragma once

#include <optional>
#include <string>
#include <vector>

#include <pc/rss/Enclosure.hpp>
#include <pc/rss/detail/RSSNode.hpp>

namespace pc::rss
{
   struct Item : RssNode
   {
      OptParam title() const noexcept
      {
         return value<Param>("title");
      }
      OptParam link() const noexcept
      {
         return value<Param>("link");
      }
      OptParam description() const noexcept
      {
         return value<Param>("description");
      }
      OptParam author() const noexcept
      {
         return value<Param>("author");
      }
#ifdef PC_RSS_ENABLE_GENERATOR_FUNCTIONS
      detail::generator<std::string> category() const noexcept
      {
         for (auto const& categoryNode : node.children("category"))
            co_yield categoryNode.text().as_string();
      }
#endif
      OptParam comments() const noexcept
      {
         return value<Param>("comments");
      }
      OptParam guid() const noexcept
      {
         return value<Param>("guid");
      }
      OptParam pubDate() const noexcept
      {
         return value<Param>("pubDate");
      }
      OptParam source() const noexcept
      {
         return value<Param>("source");
      }
      Enclosure enclosure() const noexcept
      {
         return {child("enclosure")};
      }
   };
} // namespace pc::rss
