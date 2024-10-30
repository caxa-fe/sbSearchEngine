#include "lib/lib.h"
#include "lib/converterJSON.h"
#include "lib/invertedindex.h"
#include "lib/request.h"
#include "lib/searchserver.h"
#include <iostream>


int main()
{
    setlocale(LC_ALL, "");
    sbse::hello();
    

    ConverterJSON::MakeConfig();

    std::vector<std::string> docsFileNames = ConverterJSON::GetDocsFileNames();
    InvertedIndex invIdx(docsFileNames);
    //invIdx.UpdateDocumentBase(docsFileNames);
    SearchServer::MakeSearch(invIdx);
    return 0;
}


/* 
TEST(sample_test_case, sample_test)
{
EXPECT_EQ(1, 1);
}
 */