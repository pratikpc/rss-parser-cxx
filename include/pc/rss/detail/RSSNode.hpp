#pragma once

#include <pc/rss/detail/XML.hpp>

namespace pc::rss
{
   namespace
   {
      using nodeT = pugi::xml_node;
   }
} // namespace pc::rss
namespace pc::rss
{
   namespace
   {
      namespace detail
      {
#ifndef PUGIXML_NO_STL
         std::string ToString(nodeT const& node)
         {
            std::ostringstream stream;
            node.print(stream, "", pugi::format_raw);
            return stream.str();
         }
#endif
      } // namespace detail
   }    // namespace
} // namespace pc::rss

namespace pc::rss
{
   struct RssNode
   {
    protected:
      nodeT node;

    public:
      RssNode(nodeT const& node) : node{node} {}
      RssNode(nodeT&& node) : node{std::move(node)} {}

      template <typename OutputT>
      std::optional<OutputT> value(std::string_view param) const noexcept
          requires(std::is_same_v<OutputT, std::string> || std::is_integral_v<OutputT>)
      {
         auto nodeV = child(param);
         if (!nodeV)
            return std::nullopt;
         nodeV = nodeV.first_child();
         if (nodeV.type() == pugi::xml_node_type::node_pcdata)
         {
            auto const text = nodeV.text();
            if constexpr (std::is_same_v<OutputT, std::string>)
               return text.as_string();
            if constexpr (std::is_integral_v<OutputT>)
               return static_cast<OutputT>(text.as_ullong());
         }
#ifndef PUGIXML_NO_STL
         if constexpr (std::is_same_v<OutputT, std::string>)
            return detail::ToString(nodeV);
#endif
         return std::nullopt;
      }
      nodeT child(std::string_view param) const noexcept
      {
         auto const nodeV = node.child(std::data(param));
         return nodeV;
      }

      template <typename T>
      std::optional<T> attribute(std::string_view param) const noexcept
          requires(std::is_same_v<T, std::string> || std::is_integral_v<T>)
      {
         auto const attribute = node.attribute(std::data(param));
         if (attribute.empty())
            return std::nullopt;
         if constexpr (std::is_same_v<T, std::string>)
            return attribute.as_string();
         if constexpr (std::is_integral_v<T>)
            return attribute.as_ullong();
      }

      explicit operator bool() const noexcept
      {
         return has_value();
      }
      bool has_value() const noexcept
      {
         return !node.empty();
      }
   };
} // namespace pc::rss