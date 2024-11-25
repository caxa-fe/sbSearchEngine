#include "../include/config.h"
#include "gtest/gtest.h"



TEST(ConfigFileTest, readConfig) {
    // Arrange
    std::ifstream in(std::string{CONFIG_FILE});       
    // Act
    const bool isOpen = in.is_open();
    in.close(); 
    // Assert
    ASSERT_TRUE(isOpen);
}
TEST(RequestFileTest, readRequest) {
    // Arrange
    std::ifstream in(std::string{REQUEST_FILE});       
    // Act
    const bool isOpen = in.is_open();
    in.close(); 
    // Assert
    ASSERT_TRUE(isOpen);
}

struct ConfigRequestTest : public testing::Test {
    Config& json = Config::GetLink();
    nlohmann::json cfg = json.getConfig();
    nlohmann::json rqt = json.getRequest();
};

TEST_F(ConfigRequestTest, ContainsVersion) {
    // Act
    const bool containsVersion = cfg.contains("version");
    // Assert
    EXPECT_TRUE(containsVersion);
}
TEST_F(ConfigRequestTest, IsVersionCorrect) {

    EXPECT_TRUE(PROJECT_VERSION == cfg["version"]);
}

TEST_F(ConfigRequestTest, getResponsesLimit) {

    EXPECT_TRUE(cfg.contains("max_responses") && cfg["max_responses"] == 9);
}

TEST_F(ConfigRequestTest, getRequests) {

    ASSERT_TRUE(rqt.contains("requests") && rqt["requests"].size() == 4 );
}

TEST_F(ConfigRequestTest, ContainsFiles) {
    ASSERT_TRUE(cfg.contains("files") && cfg["files"].size() == 3 );
}

TEST_F(ConfigRequestTest, getFiles) {
    // Arrange
    bool filesOK {true};
           
    // Act
    for (auto & f : cfg["files"])
    {
        std::ifstream in(f);
        filesOK = filesOK && in.is_open();
    }
    // Assert
    ASSERT_TRUE(filesOK);
}

