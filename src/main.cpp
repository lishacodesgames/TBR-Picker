#include <pch/Precompiled.h>

#include "external/csv.hpp"
using namespace csv;

struct Book {
   std::uint64_t id;
   std::string title;
   std::vector<std::string> authors;
   float myRating; /// 0 = unrated
   int pages;
   int year; /// Original Publication Year
   std::chrono::year_month_day dateAdded;
   std::vector<std::string> shelves;
   std::string exclusiveShelf;
   std::uint32_t readCount;

   void print() const noexcept {
      std::println("ID: {}", id);
      std::println("Title: {}", title);
      std::println("Authors: {}", authors);

      std::println("My Rating: {}", myRating);
      std::println("Pages: {}", pages);
      std::println("Year Published: {}", year);
      std::println("Date Added: {}", dateAdded);
      std::println("Shelves: {}", shelves);
      std::println("Exclusive Shelf: {}", exclusiveShelf);
      std::println("Read Count: {}", readCount);
   }
};

void printTitle() {
   std::print("\033[2J\033[H");
   std::println("----------------------- TBR PICKER -----------------------");
}

std::vector<std::string> getAuthors(const CSVRow& row) {
   std::vector<std::string> authors = { row["Author"].get<std::string>() };

   std::stringstream others(row["Additional Authors"].get<std::string>());
   std::string author;
   while(std::getline(others, author, ',')) {
      // Trim leading whitespace if it exists
      size_t white = author.find_first_not_of(" \t");
      if (white != std::string::npos)
         author = author.substr(white);

      authors.push_back(std::move(author));
   }

   return authors;
}

std::chrono::year_month_day getDateAdded(const CSVRow& row) {
   std::string rawDate = row["Date Added"].get<std::string>();
   int year = std::stoi(rawDate.substr(0, 4)); // 4 chars from 0th index
   int month = std::stoi(rawDate.substr(5, 2));
   int day = std::stoi(rawDate.substr(8, 2));

   return std::chrono::year{ year } / std::chrono::month{ static_cast<unsigned int>(month) } / std::chrono::day{ static_cast<unsigned int>(day) };
}

std::vector<std::string> getBookshelves(const CSVRow& row) {
   std::vector<std::string> shelves{};

   std::stringstream csvShelves(row["Bookshelves"].get<std::string>());
   std::string shelf;
   while(std::getline(csvShelves, shelf, ',')) {
      // Trim leading whitespace if it eists
      size_t white = shelf.find_first_not_of(" \t");
      if(white != std::string::npos)
         shelf = shelf.substr(white);

      shelves.push_back(std::move(shelf));
   }

   return shelves;
}

int main() {
   std::println("Loading your library...");
   CSVReader reader("assets/goodreads_library_export.csv");
   std::vector<Book> library{};

   for(CSVRow& row : reader) {
      Book book{
         .id = row["Book Id"].get<std::uint64_t>(),
         .title = row["Title"].get<std::string>(),
         .authors = getAuthors(row),
         .myRating = row["My Rating"].get<float>(),
         .pages = 0,
         .year = 0,
         .dateAdded = getDateAdded(row),
         .shelves = getBookshelves(row),
         .exclusiveShelf = row["Exclusive Shelf"].get<std::string>(),
         .readCount = row["Read Count"].get<std::uint32_t>()
      };

      try {
         book.pages = row["Number of Pages"].get<int>();
      } catch(const std::runtime_error& e) {}

      try {
         book.year = row["Original Publication Year"].get<int>();
      } catch(const std::runtime_error& e) {}

      library.push_back(std::move(book));
   }

   printTitle();
   std::println("Library loaded successfully!");
   
   while(true) {
      std::println("0. Quit");
      std::println("1. Pick a random book");
      std::println("2. See my library");
      std::print("What would you like to do? ");

      std::string ans;
      std::getline(std::cin, ans);
      std::transform(ans.begin(), ans.end(), ans.begin(), [](unsigned char c) { return std::tolower(c); }); // lowercase string
      
      if(ans == "0" || ans == "q" || ans == "quit") {
         printTitle();
         std::println("Goodbye!");
         break;

      } else if(ans == "1" || ans == "pick" || ans == "random book" || ans == "random") {
         bool pick = true;
         while(pick) {
            printTitle();
            std::println("Picking a random book...");
   
            srand(time(0));
            int index = rand() % library.size();
   
         pick_menu: // sue me for using labels
            std::println("\nBook chosen! Your next read is:\n");
            library[index].print();
            std::println("\n-----------------------------------------");

            std::println("0. Thanks. I'm gonna read this.");
            std::println("1. I hate this. Give me another.");
            std::print("Hmmm? ");
            std::getline(std::cin, ans);
            std::transform(ans.begin(), ans.end(), ans.begin(), [](unsigned char c) { return std::tolower(c); }); // lowercase string

            if(ans == "0") {
               printTitle();
               std::println("Glad you liked it!\n");
               pick = false;
               break;

            } else if(ans == "1") {
               continue;

            } else {
               printTitle();
               std::println("'{}' is not a valid choice.\n", ans);
               goto pick_menu;
            }
         }

      } else if(ans == "2" || ans == "see" || ans == "lib" || ans == "library") {
         printTitle();
         std::println("Your library has {} books.\n", library.size());

         for(size_t i = 0; i < library.size(); i++) {
            library[i].print();
            std::string c;
            std::getline(std::cin, c);
            if(c == "q")
               break;

            std::println("\n-----------------------------------------");
         }

         printTitle();

      } else {
         printTitle();
         std::println("'{}' is not a valid choice.\n", ans);
         continue;
      }
   }
}