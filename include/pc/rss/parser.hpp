#pragma once

#include <optional>
#include <sstream>
#include <string>
#include <string_view>

#include <pugixml.hpp>

#include <pc/rss/Channel.hpp>
#include <pc/rss/Item.hpp>
#include <pc/rss/parser.hpp>

#if __has_include("generator")
#   include <generator>
#   define PC_RSS_PARSER_USE_STD_GENERATOR
#   define PC_RSS_ENABLE_GENERATOR_FUNCTIONS
#elif __has_include("experimental/generator")
#   include <experimental/generator>
#   define PC_RSS_PARSER_USE_EXPERIMENTAL_GENERATOR
#   define PC_RSS_ENABLE_GENERATOR_FUNCTIONS
#endif

namespace pc::rss
{
   namespace
   {
      using docT  = pugi::xml_document;
      using nodeT = pugi::xml_node;

      using Param    = std::string;
      using IntParam = std::size_t;

#ifdef PC_RSS_PARSER_USE_STD_GENERATOR
      using std::generator;
#endif
#ifdef PC_RSS_PARSER_USE_EXPERIMENTAL_GENERATOR
      using std::experimental::generator;
#endif
   } // namespace

   namespace
   {
      namespace detail
      {
         nodeT ChannelNode(docT const& doc)
         {
            auto const rssNode = doc.child("rss");
            if (!rssNode)
               return {};
            auto const channelNode = rssNode.child("channel");
            if (!channelNode)
               return {};
            return channelNode;
         }

         template <typename OutputT>
         std::optional<OutputT> Extract(nodeT const&, std::string_view) requires(
             !(std::is_same_v<OutputT, std::string> || std::is_integral_v<OutputT>))
         {
            static_assert(false);
         }

#ifndef PUGIXML_NO_STL
         std::string ToString(nodeT const& node)
         {
            std::ostringstream stream;
            node.print(stream, "", pugi::format_raw);
            return stream.str();
         }
#endif

         template <typename OutputT>
         std::optional<OutputT>
             Extract(nodeT const& parentNode, std::string_view type) requires(
                 std::is_same_v<OutputT, std::string> || std::is_integral_v<OutputT>)
         {
            auto node = parentNode.child(std::data(type));
            if (!node)
               return std::nullopt;
            node = node.first_child();
            if (node.type() == pugi::xml_node_type::node_pcdata)
            {
               auto const text = node.text();
               if constexpr (std::is_same_v<OutputT, std::string>)
                  return text.as_string();
               if constexpr (std::is_integral_v<OutputT>)
                  return static_cast<OutputT>(text.as_ullong());
            }
#ifndef PUGIXML_NO_STL
            if constexpr (std::is_same_v<OutputT, std::string>)
               return ToString(node);
#endif
            return std::nullopt;
         }

         template <>
         std::optional<Enclosure> Extract<Enclosure>(nodeT const&     parentNode,
                                                     std::string_view type)
         {
            auto const node = parentNode.child(std::data(type));
            if (!node)
               return std::nullopt;
            Enclosure enclosure;
            enclosure.type = node.attribute("type").as_string();
            enclosure.url  = node.attribute("url").as_string();
            enclosure.length =
                static_cast<IntParam>(node.attribute("length").as_ullong());
            return enclosure;
         }

         template <>
         std::optional<Image> Extract<Image>(nodeT const&     parentNode,
                                             std::string_view type)
         {
            auto const node = parentNode.child(std::data(type));
            if (!node)
               return std::nullopt;
            Image image;
            image.link        = Extract<Param>(node, "link").value();
            image.url         = Extract<Param>(node, "url").value();
            image.title       = Extract<Param>(node, "title").value();
            image.width       = Extract<Param>(node, "width");
            image.height      = Extract<Param>(node, "height");
            image.description = Extract<Param>(node, "description");
            return image;
         }

         Item Extract(nodeT const& node, bool extractCategory = true)
         {
            Item item{};
            item.title       = Extract<Param>(node, "title");
            item.link        = Extract<Param>(node, "link");
            item.description = Extract<Param>(node, "description");
            item.comments    = Extract<Param>(node, "comments");
            item.guid        = Extract<Param>(node, "guid");
            item.pubDate     = Extract<Param>(node, "pubDate");
            item.source      = Extract<Param>(node, "source");
            item.enclosure   = Extract<Enclosure>(node, "enclosure");

            if (extractCategory)
               for (auto const& categoryNode : node.children("category"))
                  item.category.push_back(categoryNode.text().as_string());
            return item;
         }

         template <typename OutputT>
         OutputT Extract(nodeT const&);
         template <>
         Item Extract<Item>(nodeT const& node)
         {
            return Extract(node, true);
         }
         template <>
         Channel Extract<Channel>(nodeT const& node)
         {
            Channel channel;

            channel.title          = Extract<Param>(node, "title").value();
            channel.link           = Extract<Param>(node, "link").value();
            channel.description    = Extract<Param>(node, "description").value();
            channel.language       = Extract<Param>(node, "language");
            channel.copyright      = Extract<Param>(node, "copyright");
            channel.managingEditor = Extract<Param>(node, "managingEditor");
            channel.webMaster      = Extract<Param>(node, "webMaster");
            channel.pubDate        = Extract<Param>(node, "pubDate");
            channel.generator      = Extract<Param>(node, "generator");
            channel.docs           = Extract<Param>(node, "docs");
            channel.ttl            = Extract<IntParam>(node, "ttl");
            channel.image          = Extract<Image>(node, "image");
            // channel.cloud     = ExtractItem<Param>(node, "cloud");

            return channel;
         }
      } // namespace detail
   }    // namespace

   struct Parser
   {
    private:
      docT  doc;
      nodeT channelNode;

    public:
      Channel channel;

      bool extractCategoryFromItems = true;

#ifdef PC_RSS_ENABLE_GENERATOR_FUNCTIONS
      generator<std::string> category() const noexcept
      {
         for (auto const& node : channelNode.children("category"))
            co_yield node.text().as_string();
      }
      generator<std::string> skipDays() const noexcept
      {
         for (auto const& node : channelNode.child("skipDays").children("day"))
            co_yield node.text().as_string();
      }
      generator<Item> items() const noexcept
      {
         for (auto const& node : channelNode.children("item"))
         {
            auto const item = detail::Extract(node, extractCategoryFromItems);
            co_yield item;
         }
      }
      generator<unsigned int> skipHours() const noexcept
      {
         for (auto const& node : channelNode.child("skipHours").children("hour"))
            co_yield node.text().as_uint();
      }
#endif

      pugi::xml_parse_result Load(std::string_view text)
      {
         return doc.load_string(std::data(text));
      }
      pugi::xml_parse_result Load(void* const buffer, std::size_t const size)
      {
         return doc.load_buffer(buffer, size);
      }
#ifndef PUGIXML_NO_STL
      template <typename CharT>
      pugi::xml_parse_result
          Load(std::basic_istream<CharT, std::char_traits<CharT>>& stream) requires(
              std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>)
      {
         return doc.load(stream);
      }
#endif
      bool Parse(std::string_view text)
      {
         auto const result = Load(text);
         if (!result)
            return false;
         return Parse();
      }

      bool Parse()
      {
         channelNode = detail::ChannelNode(doc);
         if (!channelNode)
            return false;

         channel = detail::Extract<Channel>(channelNode);
         return true;
      }
   };
} // namespace pc::rss