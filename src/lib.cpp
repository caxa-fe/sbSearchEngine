#include "lib.h"
#include <iostream>

void sbse::showTitle(char lang)
{
    if (lang == '2')
    {
        std::wcout << L"\n===  Поиск по файлам Skillbox  ===" << std::endl;
        std::wcout << L"           Версия " << PROJECT_VERSION << "\n" << std::endl;
    }
    else
    {
        std::wcout << "\n===  Skillbox Search Engine Project  ===" << std::endl;
        std::wcout << "              Version " << PROJECT_VERSION << "\n" << std::endl;        
    }
}

void sbse::selectLanguage(char & lang)
{
    while(true)
    {
        std::wcout << "Select the interface language" << std::endl; 
        std::wcout << "1 - EN" << std::endl; 
        std::wcout << "2 - RU" << std::endl;
        std::wcout << "9 - Quit" << std::endl;

        std::cin >> lang;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();

        if ( lang == '1' || lang == '2')
        {
            break;
        }

        else if ( lang == '9' )
        {
            std::wcout << "Exiting..." << std::endl;
            exit(0);
        }
        else 
        {
            std::wcout << "Make yuor choice, please" << std::endl;
        }
    }
}

void sbse::translate(std::wstring & msg, const char & lang)
{
    std::map<std::wstring, std::wstring> dict 
    {
        {L"1 - Show configuration", L"1 - Показать конфигурацию"},
        {L"2 - Build a new index", L"2 - Построить новый индекс"},
        {L"3 - Make search", L"3 - Произвести поиск"},
        {L"8 - Переключиться на русский", L"8 - Switch to English"},
        {L"9 - Quit", L"9 - Выйти"},
        {L"    -- MAIN MENU --", L"    -- Главное меню --"},
        {L"Index is empty. You have to build index first.", L"Индекс пуст. Сперва его надо построить."},
        {L"Common Index is empty. Have to build it first.",  L"Общий Индекс пуст. Сперва его надо построить."},
        {L"Building index...", L"Строим индекс..."},
        {L"Done.", L"Готово."},
        {L"Something went wrong. Index is empty.", L"Что-то пошло не так. Индекс пуст."},
        {L"Answer saved in file: ", L"Результат сохранен в файле: "}
    };
    if (lang == '1') return;
    else if (dict.count(msg))
    {
        msg = dict[msg];
    }
    return;
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

void sbse::toLowerRu(wchar_t &ch)
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
void sbse::string2Words(std::wstring &str, std::vector<std::wstring> &docWords)
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


