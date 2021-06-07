#pragma once

#include <optional>
#include <string>

#include <pc/rss/Image.hpp>
#include <pc/rss/Item.hpp>
#include <pc/rss/detail/RSSNode.hpp>

namespace pc::rss
{
   namespace
   {
      using OptIntParam = std::optional<std::size_t>;
   } // namespace

   struct Channel : public RssNode
   {
      Param title() const
      {
         return value<Param>("title").value();
      }
      Param link() const
      {
         return value<Param>("link").value();
      }
      Param description() const
      {
         return value<Param>("description").value();
      }
      OptParam language() const noexcept
      {
         return value<Param>("language");
      }
      OptParam copyright() const noexcept
      {
         return value<Param>("copyright");
      }
      OptParam managingEditor() const noexcept
      {
         return value<Param>("managingEditor");
      }
      OptParam webMaster() const noexcept
      {
         return value<Param>("webMaster");
      }
      OptParam pubDate() const noexcept
      {
         return value<Param>("pubDate");
      }
      OptParam generator() const noexcept
      {
         return value<Param>("generator");
      }
      OptParam docs() const noexcept
      {
         return value<Param>("docs");
      }
      OptIntParam ttl() const noexcept
      {
         return value<IntParam>("ttl");
      }
      Image image() const noexcept
      {
         return {child("image")};
      }

#ifdef PC_RSS_ENABLE_GENERATOR_FUNCTIONS
      detail::generator<Param> category() const noexcept
      {
         for (auto const& categoryNode : node.children("category"))
            co_yield categoryNode.text().as_string();
      }
      detail::generator<Param> skipDays() const noexcept
      {
         for (auto const& nodeV : child("skipDays").children("day"))
            co_yield nodeV.text().as_string();
      }
      detail::generator<Item> items() const noexcept
      {
         for (auto const& nodeV : node.children("item"))
         {
            Item const item{nodeV};
            co_yield item;
         }
      }
      detail::generator<unsigned int> skipHours() const noexcept
      {
         for (auto const& nodeV : child("skipHours").children("hour"))
            co_yield nodeV.text().as_uint();
      }
#endif
   };
} // namespace pc::rss