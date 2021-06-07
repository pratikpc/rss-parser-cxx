#pragma once

#include <string_view>

#include <pc/rss/Channel.hpp>
#include <pc/rss/Item.hpp>

namespace pc::rss
{
   namespace
   {
      using docT = pugi::xml_document;
   }
} // namespace pc::rss

namespace pc::rss
{
   struct Parser
   {
    private:
      docT doc;

    public:
      pugi::xml_parse_result Load(std::string_view   text,
                                  unsigned int const options = pugi::parse_default)
      {
         return doc.load_string(std::data(text), options);
      }
      pugi::xml_parse_result Load(void* const        buffer,
                                  std::size_t const  size,
                                  unsigned int const options = pugi::parse_default)
      {
         return doc.load_buffer(buffer, size, options);
      }

      Parser(std::string_view text)
      {
         Load(text);
      }
      Parser() = default;
      explicit operator bool() const noexcept
      {
         return !doc.empty();
      }
#ifndef PUGIXML_NO_STL
      template <typename CharT>
      pugi::xml_parse_result
          Load(std::basic_istream<CharT>& stream,
               unsigned int const         options =
                   pugi::parse_default) requires(std::is_same_v<CharT, char> ||
                                                 std::is_same_v<CharT, wchar_t>)
      {
         return doc.load(stream, options);
      }
#endif
      rss::Channel Channel(std::string_view text)
      {
         Load(text);
         return Channel();
      }

      rss::Channel Channel()
      {
         auto const channelNode = doc.child("rss").child("channel");
         return {channelNode};
      }
   };
} // namespace pc::rss