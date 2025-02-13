#include "parser.h"
#include <map>

static bool valid_index(int index, int len)
{
    return index < len - 1;
}

bool is_int(int index, std::string_view& file_content)
{
    return valid_index(index, file_content.length()) && file_content[index] == 'i' && (file_content[index+1] >= '0' && file_content[index+1] <= '9' || file_content[index+1] == '-');
}

bool is_string(int index, std::string_view& file_content)
{
    return valid_index(index, file_content.length()) && file_content[index] >= '0' && file_content[index] <= '9';
}

bool is_list(int index, std::string_view& file_content)
{
    return valid_index(index, file_content.length()) && file_content[index] == 'l';
}

bool is_dictionary(int index, std::string_view& file_content)
{
    return valid_index(index, file_content.length()) && file_content[index] == 'd';
}

BencodeElementPtr parse_int(int& index, std::string_view& file_content)
{
    int num_len = 0, num = 0;
    int length = file_content.length();

    int j = index;
    int had_minus = 0;
    while(file_content[++j] != 'e' && j < length) {
        if (file_content[j] == '-')
            had_minus = 1;
        else
            num_len++;
    }

    int num_len_cpy = num_len + had_minus;
    for(j = 0; j < num_len_cpy; j++) {
        if(file_content[j+index+1] != '-') {
            num += (file_content[j+index+1] - '0') * static_cast<int>(pow(10,num_len-1));
            num_len--;
        }
    }
    if(had_minus == 1)
        num *= -1;

    if(index + j + 2 <= length)
        index += j + 2;
    else
        index = -1;


    return std::make_shared<BencodeElement>(num);
}

BencodeElementPtr parse_string(int& index, std::string_view& file_content)
{
    int file_legnth = file_content.length();
    int len_size = 0, i = index, len = 0;

    while(file_content[i++] != ':' && i < file_legnth) len_size++;
    int start_index = i, len_size_cpy = len_size;

    for(i = 0; i < len_size_cpy; i++) {
        len += (file_content[i+index] - '0') * static_cast<int>(pow(10, len_size-1));
        len_size--;
    }
    /*
    if(start_index + len <= file_legnth)
        index = start_index+len;
    else
        index = -1;
    */
    index = start_index + len-1;

    return std::make_shared<BencodeElement>(file_content.substr(start_index, len));
}

BencodeElementPtr parse_list(int& index, std::string_view& file_content)
{
    std::vector<BencodeElementPtr> list;

    // +1 if starting at literal 'l'
    int i = index;
    while(i < file_content.size() && file_content[i++] != 'e' && i != -1) {
        if(is_int(i, file_content))
            list.push_back(parse_int(i, file_content));
        else if(is_string(i, file_content))
            list.push_back(parse_string(i, file_content));
        else if(is_list(i, file_content))
            list.push_back(parse_list(i, file_content));
        else if(is_dictionary(i, file_content))
            list.push_back(parse_dictionary(i, file_content));
        else
            return nullptr;
    }

    index = i;

    return std::make_shared<BencodeElement>(list);
}

BencodeElementPtr parse_dictionary(int& index, std::string_view& file_content)
{
    std::map<std::variant<int, std::string_view>, BencodeElementPtr> map;

    int i = index;
    std::variant<int, std::string_view> key;
    BencodeElementPtr value;
    bool searching_key = true;

    while(i < file_content.size() && file_content[i++] != 'e' && i != -1) {
        if(searching_key) {
            if(is_int(i, file_content)) {
                int* k = std::get_if<int>(&parse_int(i, file_content)->value);
                if(!k)
                    return nullptr;
                key = *k;
            }
            else if(is_string(i, file_content)) {
                std::string_view* k = std::get_if<std::string_view>(&parse_string(i, file_content)->value);
                if(!k)
                    return nullptr;
                key = *k;
            }
            else
                return nullptr;
            searching_key = false;
        } else {
            if(is_int(i, file_content))
                value = parse_int(i, file_content);
            else if(is_string(i, file_content))
                value = parse_string(i, file_content);
            else if(is_list(i, file_content))
                value = parse_list(i, file_content);
            else if(is_dictionary(i, file_content))
                value = parse_dictionary(i, file_content);
            else
                return nullptr;
            searching_key = true;
        }
        map[key] = value;
    }

    index = i;

    return std::make_shared<BencodeElement>(map);
}

BencodeElementPtr parse_file(std::string_view file_content)
{
    BencodeElementPtr parsed;

    int index = 0;
    if(is_dictionary(index, file_content))
        parsed = parse_dictionary(index, file_content);
    else
        return nullptr;

    return parsed;
}
