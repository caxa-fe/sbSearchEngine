#include "textdoc.h"


size_t TextDoc::count {0};

TextDoc::TextDoc(std::string a_textFileName)
{
    docID = count;
    ++count;
    textFileName = a_textFileName;

    std::string str = ReadFile(a_textFileName);
    std::wstring wstr = sbse::utf8_utf16(str);

    sbse::String2Words(wstr,docWords);
  
}

size_t TextDoc::GetDocID()
{
    return docID;
}

std::string TextDoc::GetTextFileName()
{
    return textFileName;
}

std::vector<std::wstring>const & TextDoc::GetDocWords()
{
    return docWords;
}

// Read text from file
std::string TextDoc::ReadFile(std::string fileName)
{
    std::string text {};
    std::string line {};
    try
    {
        std::ifstream in(fileName);
        if (!in.is_open())
        {
            throw std::string{"WARNING: Can't open " + fileName + " for reading. Return empty text."};
        }
        
        while (std::getline(in, line))
        {
            text += "\n" + line;
        }
        in.close();
        
    }
    catch (std::string error_message)
    {
        std::cout << error_message << std::endl;
    }

    return text;
}

