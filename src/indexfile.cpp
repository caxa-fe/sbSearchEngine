#include "indexfile.h"

IndexFile::IndexFile(std::string & n_fName, size_t n_docID) : fName(n_fName), docID(n_docID), fileReadOK(false) 
{
    std::string text = ReadFile(fName);
    if(!fileReadOK) return;

    std::wstring wtext = sbse::utf8_utf16(text);

    for (size_t i = 0; i < wtext.length(); ++i)   
    {
        std::towlower(wtext[i]);    // Все латинские символы в нижний регистр
        sbse::toLowerRu(wtext[i]);    // Все русские символы в нижний регистр
    }

    std::vector<std::wstring> docWords;
    sbse::string2Words(wtext, docWords);
    for (auto &word : docWords)
    {
        if (countWords.count(word))
        {
            ++countWords[word];
        }
        else
        {
            countWords[word] = 1;
        }
        
    }

}
IndexFile::~IndexFile(){}

const std::map<std::wstring, size_t >& IndexFile::GetCountWords() const
{
    return countWords;
}
// Read text from file
std::string IndexFile::ReadFile(std::string & fName)
{
    std::string text {};
    std::string line {};
    try
    {
        std::ifstream in(fName);
        if (!in.is_open())
        {
            std::wstring wfName(fName.begin(), fName.end());
            throw std::wstring{L"WARNING: Can't open " + wfName + L" for reading. Return empty text."};
        }
        
        while (std::getline(in, line))
        {
            text += "\n" + line;
        }
        in.close();
        fileReadOK = true;
    }
    catch (std::wstring error_message)
    {
        std::wcout << error_message << std::endl;
        fileReadOK = false;
    }

    return text;
}
const bool & IndexFile::IsFileReadOK() const
{
    return fileReadOK;
}
const size_t & IndexFile::GetDocID() const
{
    return docID;
}

const std::string & IndexFile::GetFileName() const
{
    return fName;
}
