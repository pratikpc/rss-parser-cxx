#pragma once

#include <optional>
#include <pc/rss/detail/RSSNode.hpp>
#include <string>

namespace pc::rss
{
   struct Image : public RssNode
   {
      Param title() const
      {
         return value<Param>("title").value();
      }
      Param link() const
      {
         return value<Param>("link").value();
      }
      Param url() const
      {
         return value<Param>("url").value();
      }

      OptParam width() const noexcept
      {
         return value<Param>("width");
      }
      OptParam height() const noexcept
      {
         return value<Param>("height");
      }
      OptParam description() const noexcept
      {
         return value<Param>("description");
      }
   };
} // namespace pc::rss