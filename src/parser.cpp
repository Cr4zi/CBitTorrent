#include "parser.h"

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
