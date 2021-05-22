#include <iostream>

#include <pc/rss/parser.hpp>

std::string const str{R"(
<rss version="2.0">
<channel>
<title>FeedForAll Sample Feed</title>
<description>RSS is a fascinating technology. The uses for RSS are expanding daily. Take a closer look at how various industries are using the benefits of RSS in their businesses.</description>
<link>http://www.feedforall.com/industry-solutions.htm</link>
<category domain="www.dmoz.com">Computers/Software/Internet/Site Management/Content Management</category>
<copyright>Copyright 2004 NotePage, Inc.</copyright>
<docs>http://blogs.law.harvard.edu/tech/rss</docs>
<language>en-us</language>
<lastBuildDate>Tue, 19 Oct 2004 13:39:14 -0400</lastBuildDate>
<managingEditor>marketing@feedforall.com</managingEditor>
<pubDate>Tue, 19 Oct 2004 13:38:55 -0400</pubDate>
<webMaster>webmaster@feedforall.com</webMaster>
<generator>FeedForAll Beta1 (0.0.1.8)</generator>
<image>
<url>http://www.feedforall.com/ffalogo48x48.gif</url>
<title>FeedForAll Sample Feed</title>
<link>http://www.feedforall.com/industry-solutions.htm</link>
<description>FeedForAll Sample Feed</description>
<width>48</width>
<height>48</height>
</image>
<item>
<title><b><div>RSS Solutions for Restaurants</div></b></title>
<description>Hey</description>
<link>http://www.feedforall.com/restaurant.htm</link>
<category domain="www.dmoz.com">Computers/Software/Internet/Site Management/Content Management</category>
<comments>http://www.feedforall.com/forum</comments>
<pubDate>Tue, 19 Oct 2004 11:09:11 -0400</pubDate>
</item>
<item>
<title>RSS Solutions for Schools and Colleges</title>
<description>Lmao</description>
<link>http://www.feedforall.com/schools.htm</link>
<category domain="www.dmoz.com">Computers/Software/Internet/Site Management/Content Management</category>
<comments>http://www.feedforall.com/forum</comments>
<pubDate>Tue, 19 Oct 2004 11:09:09 -0400</pubDate>
</item>
<item>
<title>RSS Solutions for PC Computer Service Companies</title>
<description>Fairies</description>
<link>http://www.feedforall.com/computer-service.htm</link>
<category domain="www.dmoz.com">Computers/Software/Internet/Site Management/Content Management</category>
<comments>http://www.feedforall.com/forum</comments>
<pubDate>Tue, 19 Oct 2004 11:09:07 -0400</pubDate>
</item>
<item>
<title>RSS Solutions for Governments</title>
<description>Jolly</description>
<link>http://www.feedforall.com/government.htm</link>
<category domain="www.dmoz.com">Computers/Software/Internet/Site Management/Content Management</category>
<comments>http://www.feedforall.com/forum</comments>
<pubDate>Tue, 19 Oct 2004 11:09:05 -0400</pubDate>
</item>
<item>
<title>RSS Solutions for Politicians</title>
<description>Salor</description>
<link>http://www.feedforall.com/politics.htm</link>
<category domain="www.dmoz.com">Computers/Software/Internet/Site Management/Content Management</category>
<category domain="www.dmoz.com">Computers/Software/Internet/Site Management/Roadise Management</category>
<comments>http://www.feedforall.com/forum</comments>
<pubDate>Tue, 19 Oct 2004 11:09:03 -0400</pubDate>
</item>
  <skipHours>
    <hour>0</hour>
    <hour>7</hour>
    <hour>23</hour>
  </skipHours>
  <skipDays>
    <day>Saturday</day>
    <day>Sunday</day>
  </skipDays>
</channel>
</rss>)"};

int main()
{
   namespace rss = pc::rss;

   rss::Parser parse;
   try
   {
      parse.Parse(str);
      for (rss::Item const& item : parse.items())
      {
         if (item.title.has_value())
            std::cout << "Title : " << item.title.value() << "\n";
         else
            std::cout << "Description : " << item.description.value() << "\n";
         for (std::string const& category : item.category)
            std::cout << "Category : " << category << "\n";
      }
      //std::cout << "Image : " << parse.channel.image.value().link << "\n";
      std::cout << "Skip Hours\n";
      for (unsigned int const hour : parse.skipHours())
         std::cout << "Skip this hour : " << hour << "\n";
      std::cout << "Skip Days\n";
      for (std::string const& day : parse.skipDays())
         std::cout << "Skip this day : " << day << "\n";
   }
   catch (std::exception const& ex)
   {
      std::cout << "Exception" << ex.what();
   }
   return EXIT_SUCCESS;
}
