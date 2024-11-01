#include "parser.h"

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

int parse_int(int& index, std::string_view& file_content)
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

    if(index + j <= length)
        index += j;
    else
        index = -1;

    return num;
}

std::string_view parse_string(int& index, std::string_view& file_content)
{
    int file_legnth = file_content.length();
    int len_size = 0, i = index, len = 0;

    while(file_content[i++] != ':' && i < file_legnth) len_size++;
    int start_index = i, len_size_cpy = len_size;

    for(i = index; i < len_size_cpy; i++) {
        len += (file_content[i] - '0') * static_cast<int>(pow(10, len_size-1));
        len_size--;
    }
    if(start_index + len <= file_legnth)
        index = start_index+len;
    else
        index = -1;

    return file_content.substr(start_index, start_index+len-1);
}

std::vector<std::variant<int, std::string_view>> parse_list(int& index, std::string_view& file_content)
{
    std::vector<std::variant<int, std::string_view>> list;
    // +1 if starting after literal 'l'
    int i = index + 1;
    while(file_content[i++] != 'e') {

    }
}

int parse_file(std::string_view file_content)
{
    int length = file_content.length();
    for(int i = 0; i < length; i++) {
        /* found an int */
        if(i < length - 1 && file_content[i] == 'i' && (file_content[i+1] >= '0' && file_content[i+1] <= '9' || file_content[i+1] == '-')) {
            int num = parse_int(++i, file_content);

            if(i == -1) // Failed to parse_int?
                return EXIT_FAILURE;
        }

    }

    return EXIT_SUCCESS;
}
