#include "wadparser.h"

#include <fstream>
#include <cstring>
#include <cstdio>

struct WADHeader
{
    char magic[4]; // Should either be IWAD or PWAD
    uint32_t num_lumps; // Number of lumps in a WAD
    uint32_t directory_offset; // The directory contains the names and offsets of all lumps
};

WADParser::WADParser(std::string wad_name)
{
    std::ifstream file(wad_name, std::ios::ate | std::ios::binary);

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    data = new uint8_t[size];

    file.read((char*)data, size);
    file.close();

    WADHeader* hdr = (WADHeader*)data;

    if (std::strncmp(hdr->magic, "IWAD", 4))
    {
        printf("[WADLoader]: Cannot load WAD: Invalid header\n");
        exit(1);
    }

    printf("Found WAD, %d lumps, directory at 0x%x\n", hdr->num_lumps, hdr->directory_offset);

    for (int i = 0; i < hdr->num_lumps; i++)
    {
        // Find the current directory entry
        DirectoryEntry* entry = (DirectoryEntry*)(data + hdr->directory_offset + (i * sizeof(DirectoryEntry)));

        // Print some info about the lump, and then save it
        char* name = new char[9];
        strncpy(name, entry->name, 8);
        name[8] = '\0';
        printf("Found lump %s at 0x%x, %d bytes\n", name, entry->lump_offset, entry->lump_size);
        directory.push_back(*entry);

        delete name;
    }

    printf("Searching for PLAYPAL, so we can dump it\n");

    // PLAYPAL is the color palette used by everything in DOOM. 
    // It's kind of important, so we'll maintain a pointer to it's specific entry

    playpal = nullptr;

    for (auto& entry : directory)
    {
        if (!strncmp(entry.name, "PLAYPAL", 7))
        {
            playpal = &entry;
            break;
        }
    }

    if (!playpal)
    {
        printf("[WADLoader]: Couldn't load WAD: PLAYPAL entry not found\n");
        exit(1);
    }

    printf("Found PLAYPAL at 0x%x\n", playpal->lump_offset);

    // We can calculate the number of palettes by dividing the lump size by the size of a single palette
    printf("PLAYPAL contains %d palettes\n", playpal->lump_size / 768);

    // TEMP
    // We dump PLAYPAL here, to assist in visualising it, and to make sure we have the palette format down

    // Each entry of PLAYPAL is 256*3 = 768 bytes, with each triplet of bytes representing the RGB of that color entry

    // We dump this to a PPM, because it's a very simple file format

    std::ofstream pal("pal.ppm");

    pal << "P3\n"; // The magic number
    pal << 16 << " " << 16 << "\n"; // The width and height of the image
    pal << 255 << "\n"; // The maximum value of each color component (R, G, or B). In our case, this is 255

    for (int i = 0; i < 768; i += 3)
    {
        int r = data[playpal->lump_offset + i + 0];
        int g = data[playpal->lump_offset + i + 1];
        int b = data[playpal->lump_offset + i + 2];

        pal << r << " " << g << " " << b << "\n";
    }

    pal.close();

    // Now we practice dumping sprites
    // Sprites are a bit more complicated than PLAYPAL, but here goes...

    Sprite* spr;
    DirectoryEntry* spr_entry;

    // Alright, search for TROOA1, which is the first frame of the Imp's forward animation

    for (auto& entry : directory)
    {
        if (!strncmp(entry.name, "TROOA1", 6))
        {
            spr_entry = &entry;
            break;
        }
    }

    if (!spr_entry)
    {
        printf("Couldn't find TROOA1. Are you sure this is a DOOM WAD?\n");
        exit(1);
    }

    spr = (Sprite*)(data + spr_entry->lump_offset);

    printf("TROOA1 is %dx%d pixels\n", spr->width, spr->height);

    // Following this header is a number of integers pointing to data for each column
}