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
      std::cout <<
      "ID: " << id << "\n" <<
      "Title: " << title << "\n" <<
      "Authors: ";

      for(const std::string& author : authors)
         std::cout << author << ", ";
      std::cout << "\b\b\n";

      std::cout <<
      "My Rating: " << myRating << "\n" <<
      "Pages: " << pages << "\n" <<
      "Year Published: " << year << "\n" <<
      "Date Added: " << static_cast<int>(dateAdded.year()) << "/" << static_cast<unsigned>(dateAdded.month()) << "/" << static_cast<unsigned>(dateAdded.day()) << "\n" <<
      "Shelves: ";

      for(const std::string& shelf : shelves)
         std::cout << shelf << ", ";
      std::cout << "\b\b\n";

      std::cout <<
      "Exclusive Shelf: " << exclusiveShelf << "\n" <<
      "Read Count: " << readCount << "\n";
   }
};

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

   std::cout << "Your library has " << library.size() << " books.\n\n";
   for(size_t i = 0; i < library.size(); i++) {
      library[i].print();
      std::string c;
      std::getline(std::cin, c);
      if(c == "q")
         break;

      std::cout << "\n-----------------------------------------\n";
   }
}