#include "../lib/constants.h"
#include "../google/gtest/gtest.h"
#include "../lib/lib.cpp"
#include "../lib/indexfile.cpp"

struct IndexFileTest : public testing::Test {
    IndexFile *idxFile;
    std::string fName = std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file000.txt";
    void SetUp() { idxFile = new IndexFile(fName, 11); } 
    void TearDown() { delete idxFile; } 
};
 
TEST_F(IndexFileTest, Index_file_read) {
    // Act
    const bool fileReadOK = idxFile ->IsFileReadOK();
    // Assert
    EXPECT_TRUE(fileReadOK);
}

TEST_F(IndexFileTest, Index_get_ID) {
    // Act
    const size_t id = idxFile -> GetDocID();
    // Assert
    EXPECT_EQ(id, 11);
}

TEST_F(IndexFileTest, Index_words_count) {
    // Arrange
    
    size_t count = 2;
    bool testPass {true};
    const std::map<std::wstring, size_t > wc = idxFile -> GetCountWords();

    // Act
    for (const auto &el : wc)
    {
        //std::wcout << el.first << " : " << el.second << std::endl;
        if (el.second != count )
        {
            testPass = false;
            break;
        }
    }

    // Assert
    EXPECT_TRUE(testPass);
}

struct IndexBadFileTest : public testing::Test {
    IndexFile *idxFile;
    std::string fName = std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file.txt";
    void SetUp() { idxFile = new IndexFile(fName, 11); } 
    void TearDown() { delete idxFile; } 
};

TEST_F(IndexBadFileTest, Throw) {
    // Act
    const bool fileReadOK = idxFile ->IsFileReadOK();
    // Assert
    EXPECT_FALSE(fileReadOK);
}

struct CommonIndexTest : public testing::Test {
    IndexFile *idxFile_0;
    std::string fName_0 = std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file000.txt";
    IndexFile *idxFile_1;
    std::string fName_1 = std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file001.txt";
    IndexFile *idxFile_2;
    std::string fName_2 = std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file002.txt";
    std::map<std::wstring, std::map<size_t, size_t> > commonIndex;
    void SetUp() 
    { 
        idxFile_0 = new IndexFile(fName_0, 0); 
        idxFile_1 = new IndexFile(fName_1, 1);
        idxFile_2 = new IndexFile(fName_2, 2);
        std::deque<IndexFile> deqIndex;
        deqIndex.push_back(*idxFile_0);
        deqIndex.push_back(*idxFile_1);
        deqIndex.push_back(*idxFile_2);
        
        for (auto & obj : deqIndex)
        {
            for (const auto & word : obj.GetCountWords())
            {
                commonIndex[word.first][obj.GetDocID()] += word.second;
            }
        }

    } 
    void TearDown() 
    { 
        delete idxFile_0; 
        delete idxFile_1; 
        delete idxFile_2; 
    } 
};

TEST_F(CommonIndexTest, CommonIndexLength_test) {
    // Act
    std::wcout << "-- CommonIndex" << std::endl;
    // Чтобы посмотреть содержимое commonIndex
    // раскомментируйте этот код
/* 
    for (const auto & [word, map] : commonIndex)
    {
        std::wcout << word << std::endl;
        for(const auto & m : map)
        {
            std::wcout << m.first << " : " << m.second << std::endl;
        }
        std::wcout << "-------------------------------" << std::endl;
    }
 */
    // Assert
    ASSERT_EQ(commonIndex.size(), 45);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}