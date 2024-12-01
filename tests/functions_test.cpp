#include "../include/functions.h"
#include "gtest/gtest.h"


TEST(FileExistsTest, answer_ref) {
    std::ifstream in("answer_ref.json");       
    ASSERT_TRUE(in.is_open());
    in.close();
}

struct FuncTest : public testing::Test {
    Config& json = Config::GetLink();
    nlohmann::json cfg = json.getConfig();
    nlohmann::json rqt = json.getRequest();
    std::deque<IndexFile> deqIndex;
    std::deque<Request> deqRequest;
    std::map<std::wstring, std::map<size_t, size_t> > commonIndex; // [word][docID,count]
    nlohmann::json answerRef;

    void SetUp() 
    {
        std::ifstream in("answer_ref.json"); 
        in >> answerRef;
        in.close();
        commonIndex.clear();
        makeIndex(deqIndex);
        makeCommonIndex(deqIndex, commonIndex);
        makeSearch(commonIndex, deqRequest);
        char lang = '1';
        saveResult(deqRequest, lang);
    } 
    

};

TEST_F(FuncTest, eqFiles) {
    std::string cFile = std::string{ANSWER_FILE};
    std::ifstream in(cFile);       
    ASSERT_TRUE(in.is_open());
    nlohmann::json answer;
    in >> answer;
    in.close();
    ASSERT_EQ(answer, answerRef);
}

