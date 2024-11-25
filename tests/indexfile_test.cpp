#include "../include/indexfile.h"
#include "gtest/gtest.h"



TEST(IndexFileTest, file000) {
    std::string fName = "resources/file000.txt";
    IndexFile xf = IndexFile(fName, 0);
    size_t count {0};
    for (auto & el : xf.GetCountWords())
    {
        count += el.second;
    }
    EXPECT_EQ(count, 28);
}

TEST(IndexFileTest, file001) {
    std::string fName = "resources/file001.txt";
    IndexFile xf = IndexFile(fName, 1);
    size_t count {0};
    for (auto & el : xf.GetCountWords())
    {
        count += el.second;
    } 
    EXPECT_EQ(count, 67);
}

TEST(IndexFileTest, file002) {
    std::string fName = "resources/file002.txt";
    IndexFile xf = IndexFile(fName, 2);
    size_t count {0};
    for (auto & el : xf.GetCountWords())
    {
        count += el.second;
    } 
    EXPECT_EQ(count, 102);
}
