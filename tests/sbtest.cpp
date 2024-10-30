#include "../lib/constants.h"
#include <gtest/gtest.h>
#include "../lib/lib.cpp"
#include "../lib/textdoc.cpp"
#include "../lib/invertedindex.cpp"
#include "../lib/converterJSON.cpp"
#include "../lib/request.cpp"



// Тест класса TextDoc
// Fixture
struct TextDocTest : public testing::Test 
{
     
    std::string rFile = std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file000.txt";

    TextDoc *td;
    void SetUp() { td = new TextDoc(rFile); }
    void TearDown() { delete td; }
   
};

TEST_F(TextDocTest, wordsCount)
{
    // Количество слов в тестовом файле file000.txt
    // Act
    std::vector<std::wstring>wordsVec = td -> GetDocWords();

    // Assert
    EXPECT_EQ(wordsVec.size(), 36);

}

TEST_F(TextDocTest, chehovPos)
{
    // Номер позиции слова "чехов"
    // Act
    std::vector<std::wstring>wordsVec = td -> GetDocWords();
    int k = 0;
    for (auto & w : td -> GetDocWords())
    {
        if ( w == L"чехов") break;
        ++k;
    }

    // Assert
    EXPECT_EQ(k, 19);

}


// Тест класса InvertedIndex
// Fixture
struct InvertedIndexTest : public testing::Test 
{
    std::vector<std::string> vecFileNames 
    {
        std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file000.txt",
        std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file001.txt",
        std::string{CURRENT_SOURCE_DIR} + "/tests/test_resources/file002.txt"
    }; 

    std::vector<std::string> &vecLink = vecFileNames;

    InvertedIndex *invInd;
    void SetUp() { invInd = new InvertedIndex(vecLink);}
    void TearDown() { delete invInd; }

};

TEST_F(InvertedIndexTest, wordCountInDocs1)
{
    // Количество вхождений слова "русская" во всех документах
    // Act
    const std::vector<std::pair<size_t,size_t>> vecPair = invInd -> GetWordPairVec(L"русская");
    std::string res {};
    for (auto & vp : vecPair)
    {
        res += "docID: " + std::to_string(vp.first) + " count: " + std::to_string(vp.second) + " | ";
    }

    std::string exp 
    {
        "docID: 2 count: 1 | docID: 3 count: 1 | docID: 4 count: 2 | "
    };

    // Assert
    EXPECT_STREQ(exp.c_str(), res.c_str());

}

TEST_F(InvertedIndexTest, wordCountInDocs2)
{
    // Количество вхождений слова "писатели" во всех документах
    // Отличие от предыдущего теста - слово содержится не во всех документах
    // Act
    const std::vector<std::pair<size_t,size_t>> vecPair = invInd -> GetWordPairVec(L"писатели");
    std::string res {};
    for (auto & vp : vecPair)
    {
        res += "docID: " + std::to_string(vp.first) + " count: " + std::to_string(vp.second) + " | ";
    }

    std::string exp 
    {
        "docID: 6 count: 1 | docID: 7 count: 1 | "
    };

    // Assert
    EXPECT_STREQ(exp.c_str(), res.c_str());

}


/**
 * Тест класса Request.
 * Конвертация строки запроса в wstring,
 * разбиение ее на слова
 */
TEST(RequestTest, requestTest1)
{
    // Arrange
    Request *req;
    req = new Request(std::string {"русская литература"});

    // Act
    std::set<std::wstring> wordsSet = req ->GetReqWords();
    std::vector<std::wstring>wordsVec;
    for (auto & w : wordsSet)
    {
        wordsVec.push_back(w);
    }

    // Assert
    EXPECT_TRUE((wordsVec[0] == L"литература") && (wordsVec[1] == L"русская"));
}

TEST(RequestTest, requestTest2)
{
    // Arrange
    Request *req;
    req = new Request(std::string {"Пушкин, Чехов, Булгаков, Набоков"});

    // Act
    std::set<std::wstring> wordsSet = req ->GetReqWords();
    std::vector<std::wstring>wordsVec;
    for (auto & w : wordsSet)
    {
        wordsVec.push_back(w);
    }

    // Assert
    EXPECT_TRUE((   wordsVec[0] == L"булгаков") 
                && (wordsVec[1] == L"набоков")
                && (wordsVec[2] == L"пушкин")
                && (wordsVec[3] == L"чехов")
    );
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}