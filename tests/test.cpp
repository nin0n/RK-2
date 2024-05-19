#include <iostream>
#include <string>
#include <variant>
#include <initializer_list>
#include <vector>
#include <gtest/gtest.h>

// Bookstore.cpp // Visitor Pattern
// ===========================================================================

namespace Bookstore_Example {

    class Book
    {
    private:
        std::string m_author;
        std::string m_title;
        double m_price;
        size_t m_count;

    public:
        Book(std::string author, std::string title, double price, size_t count)
            : m_author{ author }, m_title{ title }, m_price{ price }, m_count{ count } { }

        // getter / setter
        std::string getAuthor() const { return m_author; }
        std::string getTitle() const { return m_title; }
        double getPrice() const { return m_price; }
        size_t getCount() const { return m_count; }
    };

    class Movie
    {
    private:
        std::string m_title;
        std::string m_director;
        double m_price;
        size_t m_count;

    public:
        Movie(std::string title, std::string director, double price, size_t count)
            : m_title{ title }, m_director{ director }, m_price{ price }, m_count{ count } { }

        // getter / setter
        std::string getTitle() const { return m_title; }
        std::string getDirector() const { return m_director; }
        double getPrice() const { return m_price; }
        size_t getCount() const { return m_count; }
    };

    template <typename ... TMedia>
    class Bookstore
    {
    private:
        using Stock = std::vector<std::variant<TMedia ...>>;
        using StockList = std::initializer_list<std::variant<TMedia ...>>;

    public:
        explicit Bookstore(StockList stock) : m_stock{ stock } {}

        // template member method
        template <typename T>
        void addMedia(const T& media) {
            m_stock.push_back(media);
        }

        double totalBalance() {

            double total{};

            for (const auto& media : m_stock) {

                double price{};
                size_t count{};

                std::visit(
                    [&](const auto& element) {
                        price = element.getPrice();
                        count = element.getCount();
                    },
                    media
                );

                total += price * count;
            }

            return total;
        }

        size_t count() {

            size_t total{};

            for (const auto& element : m_stock) {

                size_t count{};

                std::visit(
                    [&](const auto& element) {
                        count = element.getCount();
                    },
                    element
                );

                total += count;
            }

            return total;
        }

        // -----------------------------------------------
        // demonstrating std::visit with returning a value

        double totalBalanceEx() {

            double total{};

            for (const auto& media : m_stock) {

                total += std::visit(
                    [](const auto& element) {
                        double price = element.getPrice();
                        size_t count = element.getCount();
                        return price * count;
                    },
                    media
                );
            }

            return total;
        }

        size_t countEx() {

            size_t total{};

            for (const auto& element : m_stock) {
                total += std::visit(
                    [](const auto& element) {
                        return element.getCount();
                    },
                    element
                );
            }

            return total;
        }

    private:
        Stock m_stock;
    };

    static void clientCode_01() {
        Bookstore<Book, Movie> store{
            Book{"Author1", "Book1", 10.0, 5},
            Book{"Author2", "Book2", 15.0, 3},
            Movie{"Movie1", "Director1", 20.0, 2}
        };

        std::cout << "Total balance: " << store.totalBalance() << std::endl;
        std::cout << "Total count: " << store.count() << std::endl;
        std::cout << "Total balance (Ex): " << store.totalBalanceEx() << std::endl;
        std::cout << "Total count (Ex): " << store.countEx() << std::endl;
    }
}

// Tests
TEST(BookstoreTest, TotalBalance) {
    Bookstore_Example::Bookstore<Bookstore_Example::Book, Bookstore_Example::Movie> store{
        Bookstore_Example::Book{"Author1", "Book1", 10.0, 5},
        Bookstore_Example::Book{"Author2", "Book2", 15.0, 3},
        Bookstore_Example::Movie{"Movie1", "Director1", 20.0, 2}
    };

    double expectedBalance = (10.0 * 5) + (15.0 * 3) + (20.0 * 2);
    EXPECT_DOUBLE_EQ(store.totalBalance(), expectedBalance);
}

TEST(BookstoreTest, TotalCount) {
    Bookstore_Example::Bookstore<Bookstore_Example::Book, Bookstore_Example::Movie> store{
        Bookstore_Example::Book{"Author1", "Book1", 10.0, 5},
        Bookstore_Example::Book{"Author2", "Book2", 15.0, 3},
        Bookstore_Example::Movie{"Movie1", "Director1", 20.0, 2}
    };

    size_t expectedCount = 5 + 3 + 2;
    EXPECT_EQ(store.count(), expectedCount);
}

TEST(BookstoreTest, AddMedia) {
    Bookstore_Example::Bookstore<Bookstore_Example::Book, Bookstore_Example::Movie> store{
        Bookstore_Example::Book{"Author1", "Book1", 10.0, 5},
        Bookstore_Example::Book{"Author2", "Book2", 15.0, 3}
    };

    store.addMedia(Bookstore_Example::Movie{ "Movie1", "Director1", 20.0, 2 });

    double expectedBalance = (10.0 * 5) + (15.0 * 3) + (20.0 * 2);
    EXPECT_DOUBLE_EQ(store.totalBalance(), expectedBalance);

    size_t expectedCount = 5 + 3 + 2;
    EXPECT_EQ(store.count(), expectedCount);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();

    if (testResult == 0) {
        Bookstore_Example::clientCode_01();
    }

    return testResult;
}