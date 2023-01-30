#pragma once

#include <cstdint>
#include <string>
#include <vector>

// A directory consists of these, each describes a lump
struct DirectoryEntry
{
    uint32_t lump_offset; // Offset into a file where the lump is
    uint32_t lump_size; // The size of the lump, in bytes
    char name[8]; // ASCII string. !!!WARNING!!! Not zero-terminated!
};

struct Sprite
{
    uint16_t width, height; // Self-explanatory
    uint16_t left; // The number of pixels to the left of the sprites origin that the first column is drawn
    uint16_t top; // The number of pixels above the origin the top row is drawn
};

class WADParser
{
private:
    uint8_t* data;

    // The main WAD directory, which references all lumps
    std::vector<DirectoryEntry> directory;
    
    // See WADParser(std::string) for documentation on PLAYPAL
    DirectoryEntry* playpal;
public:
    WADParser(std::string wad_name);
};