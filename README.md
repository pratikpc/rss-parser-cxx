# RSS Parser CXX

## Usage

```cpp
#include <pc/rss/parser.hpp>

int main()
{
   namespace rss = pc::rss;

   rss::Parser parse{str};
   if (!parse)
   {
      std::cout << "Load failed";
      return EXIT_FAILURE;
   }
   auto const channel = parse.Channel();
   if (!channel)
   {
      std::cout << "Parsing failed";
      return EXIT_FAILURE;
   }

   for (rss::Item const& item : channel.items())
   {
      auto const title = item.title();
      if (title)
         std::cout << "Title : " << *title << "\n";
      auto const description = item.description();
      if (description)
         std::cout << "Description : " << *description << "\n";

      for (std::string const& category : item.category())
         std::cout << "Category : " << category << "\n";
      std::cout << "================================\n\n";
   }
   auto const image = channel.image();
   if (image)
   {
      std::cout << "Image : " << image.link() << "\n";
      std::cout << "================================\n\n";
   }

   std::cout << "Skip Hours\n";
   for (unsigned int const hour : channel.skipHours())
      std::cout << "Skip this hour : " << hour << "\n";
   std::cout << "================================\n\n";

   std::cout << "Skip Days\n";
   for (std::string const& day : channel.skipDays())
      std::cout << "Skip this day : " << day << "\n";
   std::cout << "================================\n\n";

   return EXIT_SUCCESS;
}
```

## Depends on
- [pugixml](https://github.com/zeux/pugixml)  
It helps us parse RSS documents