#pragma once

#include <pc/rss/detail/RSSNode.hpp>

namespace pc::rss
{
   namespace
   {
      using Bytes = std::size_t;
   } // namespace
   struct Enclosure : public RssNode
   {
      Param type() const 
      {
         return attribute<Param>("type").value();
      }
      Param url() const 
      {
         return attribute<Param>("url").value();
      }
      Bytes length() const 
      {
         return attribute<Bytes>("url").value();
      }
   };
} // namespace pc::rss