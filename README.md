# RSS Parser CXX

## Usage

```cpp
#include <pc/rss/parser.hpp>

int main()
{
   namespace rss = pc::rss;

   // Add valid RSS
   std::string const str{R"()"};

   rss::Parser parse;
   if (!parse.Load(str))
   {
      std::cout << "Load failed";
      return EXIT_FAILURE;
   }
   if (!parse.Parse())
   {
      std::cout << "Parsing failed";
      return EXIT_FAILURE;
   }

   for (rss::Item const& item : parse.items())
   {
      if (item.title)
         std::cout << "Title : " << *item.title << "\n";

      if (item.description)
         std::cout << "Description : " << *item.description << "\n";

      for (std::string const& category : item.category)
         std::cout << "Category : " << category << "\n";
      std::cout << "================================\n\n";
   }
   if (parse.channel.image)
   {
      std::cout << "Image : " << parse.channel.image->link << "\n";
      std::cout << "================================\n\n";
   }

   std::cout << "Skip Hours\n";
   for (unsigned int const hour : parse.skipHours())
      std::cout << "Skip this hour : " << hour << "\n";
   std::cout << "================================\n\n";

   std::cout << "Skip Days\n";
   for (std::string const& day : parse.skipDays())
      std::cout << "Skip this day : " << day << "\n";
   std::cout << "================================\n\n";

   return EXIT_SUCCESS;
}
```

## Depends on
- [pugixml](https://github.com/zeux/pugixml)  
It helps us parse RSS documents