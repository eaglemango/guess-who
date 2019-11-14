#include <cassert>
#include <cstdlib>
#include <cstring>

char* GenerateSpaces(size_t spaces_count) {
    char* spaces = (char*)calloc(spaces_count, sizeof(char));
    assert(spaces);

    memset(spaces,' ', spaces_count);
    assert(spaces);

    return spaces;
}
