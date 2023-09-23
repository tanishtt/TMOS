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

#endif