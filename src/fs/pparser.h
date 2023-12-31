#ifndef PATHPARSER_H
#define PATHPARSER_H

//0:/test/college/abc.txt
//0:
struct path_root
{
    int drive_no;
    struct path_part* first;
};

//test/college/abc.txt
struct path_part
{
    const char* part;
    struct path_part* next;

};



struct path_root* pathparser_parse(const char* path, const char* current_directory_path);
void pathparser_free(struct path_root *root);

#endif