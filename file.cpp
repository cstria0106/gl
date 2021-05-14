//
// Created by Connple on 2021/05/14.
//

#include "file.h"
#include <cstdio>
#include <cstring>

char *load_file(const char *path) {
    static char s[2048];
    strcpy(s, "");

    FILE *f = fopen(path, "r");

    if(!f) {
        return nullptr;
    }

    size_t n = fread(s, sizeof(char), 2048, f);
    fclose(f);

    s[n] = 0;

    return s;
}