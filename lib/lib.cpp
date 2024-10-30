#include "lib.h"
#include <iostream>

const char* sbse::getVersion() {
    return PROJECT_VERSION;
}
const char* sbse::getProjectName()
{
    return PROJECT_NAME;
}

void sbse::hello()
{
    std::cout << "\n===  Skillbox Search Engine Project  ===" << std::endl;
    std::cout << "              Version " << getVersion() << "\n" << std::endl;
}

std::wstring sbse::utf8_utf16(const std::string& input)
{
/**
 * Look https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t#answers-header
 */
    setlocale(LC_ALL,"");
    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(input);
    
}

void sbse::ToLowerRu(wchar_t &ch)
{
    static std::map<wchar_t, wchar_t> dic 
    {
        {L'Й',L'й'},
        {L'Ц',L'ц'},
        {L'У',L'у'},
        {L'К',L'к'},
        {L'Е',L'е'},
        {L'Н',L'н'},
        {L'Г',L'г'},
        {L'Ш',L'ш'},
        {L'Щ',L'щ'},
        {L'З',L'з'},
        {L'Х',L'х'},
        {L'Ъ',L'ъ'},
        {L'Ф',L'ф'},
        {L'Ы',L'ы'},
        {L'В',L'в'},
        {L'А',L'а'},
        {L'П',L'п'},
        {L'Р',L'р'},
        {L'О',L'о'},
        {L'Л',L'л'},
        {L'Д',L'д'},
        {L'Ж',L'ж'},
        {L'Э',L'э'},
        {L'Я',L'я'},
        {L'Ч',L'ч'},
        {L'С',L'с'},
        {L'М',L'м'},
        {L'И',L'и'},
        {L'Т',L'т'},
        {L'Ь',L'ь'},
        {L'Б',L'б'},
        {L'Ю',L'ю'}
    };

    if (dic.count(ch))
    {
        ch = dic[ch];
    }
}

// Break string in words
void sbse::String2Words(std::wstring &str, std::vector<std::wstring> &docWords)
{
    setlocale(LC_ALL, "");
    std::set<wchar_t> alpha
    {
        'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m',
        L'й',L'ц',L'у',L'к',L'е',L'н',L'г',L'ш',L'щ',L'з',L'х',L'ъ',
        L'ф',L'ы',L'в',L'а',L'п',L'р',L'о',L'л',L'д',L'ж',L'э',
        L'я',L'ч',L'с',L'м',L'и',L'т',L'ь',L'б',L'ю'
    };
    str += L" ";    // Добавлен пробел для захвата в вектор последнего слова
    std::wstring word {};
    for (size_t i = 0; i < str.length(); ++i)
    {
        std::towlower(str[i]);
        sbse::ToLowerRu(str[i]);
        if (alpha.count(str[i]))
        {
            word += str[i];
        }
        else
        {
            if (!word.empty())
            {
                docWords.push_back(word);
            }
            word.clear();
        }
    }
    str.resize(str.length() -1); // Удалён добавленный пробел
}

//template<typename T>
std::string sbse::toString(size_t t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

std::string sbse::toString(double t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}


