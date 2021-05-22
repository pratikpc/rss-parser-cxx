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

      using MultiParam   = std::vector<Param>;
      using OptImage     = std::optional<Image>;
      using OptEnclosure = std::optional<Enclosure>;

#ifdef PC_RSS_PARSER_USE_STD_GENERATOR
      using std::generator;
#endif
#ifdef PC_RSS_PARSER_USE_EXPERIMENTAL_GENERATOR
      using std::experimental::generator;
#endif
   } // namespace

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
            auto const item = ExtractItem(node, extractCategoryFromItems);
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
      bool Parse(std::string_view text)
      {
         auto result = Load(text);
         if (!result)
            return false;
         return Parse();
      }

      bool Parse()
      {

         channelNode = ChannelNode(doc);
         if (!channelNode)
            return false;

         channel.title          = ExtractValue<Param>(channelNode, "title").value();
         channel.link           = ExtractValue<Param>(channelNode, "link").value();
         channel.description    = ExtractValue<Param>(channelNode, "description").value();
         channel.language       = ExtractValue<Param>(channelNode, "language");
         channel.copyright      = ExtractValue<Param>(channelNode, "copyright");
         channel.managingEditor = ExtractValue<Param>(channelNode, "managingEditor");
         channel.webMaster      = ExtractValue<Param>(channelNode, "webMaster");
         channel.pubDate        = ExtractValue<Param>(channelNode, "pubDate");
         channel.generator      = ExtractValue<Param>(channelNode, "generator");
         channel.docs           = ExtractValue<Param>(channelNode, "docs");
         channel.ttl            = ExtractValue<IntParam>(channelNode, "ttl");
         channel.image          = ExtractValue<Image>(channelNode, "image");
         // channel.cloud     = ExtractItem<Param>("cloud");

         return true;
      }

    private:
      static nodeT ChannelNode(docT& doc)
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
      static std::optional<OutputT> ExtractValue(nodeT const&, std::string_view) requires(
          !std::is_same_v<OutputT, std::string> && !std::is_integral_v<OutputT>)
      {
         static_assert(false);
      }

      template <typename OutputT>
      static std::optional<OutputT>
          ExtractValue(nodeT const& parentNode, std::string_view type) requires(
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
         if constexpr (std::is_same_v<OutputT, std::string>)
            return ToString(node);
         else
            return std::nullopt;
      }

      static std::string ToString(nodeT const& node)
      {

         std::ostringstream stream;
         node.print(stream, "", pugi::format_raw);
         return stream.str();
      }

      template <>
      static OptEnclosure ExtractValue<Enclosure>(nodeT const&     parentNode,
                                                  std::string_view type)
      {
         auto node = parentNode.child(std::data(type));
         if (!node)
            return std::nullopt;
         Enclosure enclosure;
         enclosure.type   = node.attribute("type").as_string();
         enclosure.url    = node.attribute("url").as_string();
         enclosure.length = static_cast<IntParam>(node.attribute("length").as_ullong());
         return enclosure;
      }

      template <>
      static OptImage ExtractValue<Image>(nodeT const& parentNode, std::string_view type)
      {
         auto node = parentNode.child(std::data(type));
         if (!node)
            return std::nullopt;
         Image image;
         image.link        = ExtractValue<Param>(node, "link").value();
         image.url         = ExtractValue<Param>(node, "url").value();
         image.title       = ExtractValue<Param>(node, "title").value();
         image.width       = ExtractValue<Param>(node, "width");
         image.height      = ExtractValue<Param>(node, "height");
         image.description = ExtractValue<Param>(node, "description");
         return image;
      }

      static Item ExtractItem(nodeT const& node, bool extractCategory = true)
      {
         Item item{};
         item.title       = ExtractValue<Param>(node, "title");
         item.link        = ExtractValue<Param>(node, "link");
         item.description = ExtractValue<Param>(node, "description");
         item.comments    = ExtractValue<Param>(node, "comments");
         item.guid        = ExtractValue<Param>(node, "guid");
         item.pubDate     = ExtractValue<Param>(node, "pubDate");
         item.source      = ExtractValue<Param>(node, "source");
         item.enclosure   = ExtractValue<Enclosure>(node, "enclosure");

         if (extractCategory)
            for (auto const& categoryNode : node.children("category"))
               item.category.push_back(categoryNode.text().as_string());
         return item;
      }
   };
} // namespace pc::rss