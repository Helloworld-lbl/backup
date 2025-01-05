#include "lz77/lz77.h"

int LZ77::compress(const std::string &file, const std::string &targetFile)
{
    int level = 1;
    FILE *in = fopen(file.c_str(), "rb");
    if (!in)
    {
        perror(file.c_str());
        return -1;
    }

    FILE *out = fopen(targetFile.c_str(), "wb");
    if (!out)
    {
        perror(targetFile.c_str());
        return -1;
    }

    const int magic = ULZ_MAGIC;
    fwrite(&magic, 1, sizeof(magic), out);

    ULZ *ulz = new ULZ;

    ULZ::U8 *in_buf = new ULZ::U8[BLOCK_SIZE + ULZ::EXCESS];
    ULZ::U8 *out_buf = new ULZ::U8[BLOCK_SIZE + ULZ::EXCESS];

    int raw_len;
    while ((raw_len = fread(in_buf, 1, BLOCK_SIZE, in)) > 0)
    {
        const int comp_len = ulz->Compress(in_buf, raw_len, out_buf, level);

        fwrite(&comp_len, 1, sizeof(comp_len), out);
        fwrite(out_buf, 1, comp_len, out);

        fprintf(stderr, "%lld -> %lld\r", _ftelli64(in), _ftelli64(out));
    }

    delete ulz;

    delete[] in_buf;
    delete[] out_buf;

    fclose(in);
    fclose(out);

    return 0;
}

int LZ77::decompress(const std::string &file, const std::string &targetFile)
{

    FILE *in = fopen(file.c_str(), "rb");
    if (!in)
    {
        perror(file.c_str());
        return -1;
    }

    FILE *out = fopen(targetFile.c_str(), "wb");
    if (!out)
    {
        perror(targetFile.c_str());
        return -1;
    }

    int magic;
    fread(&magic, 1, sizeof(magic), in);
    if (magic != ULZ_MAGIC)
    {
        fprintf(stderr, "Not in ULZ format!\n");
        exit(1);
    }

    ULZ *ulz = new ULZ;

    ULZ::U8 *in_buf = new ULZ::U8[BLOCK_SIZE + ULZ::EXCESS];
    ULZ::U8 *out_buf = new ULZ::U8[BLOCK_SIZE + ULZ::EXCESS];

    int comp_len;
    while (fread(&comp_len, 1, sizeof(comp_len), in) > 0)
    {
        if (comp_len < 2 || comp_len > (BLOCK_SIZE + ULZ::EXCESS) || fread(in_buf, 1, comp_len, in) != comp_len)
        {
            fprintf(stderr, "Corrupt input!\n");
            exit(1);
        }

        const int out_len = ulz->Decompress(in_buf, comp_len, out_buf, BLOCK_SIZE);
        if (out_len < 0)
        {
            fprintf(stderr, "Stream error!\n");
            exit(1);
        }

        if (fwrite(out_buf, 1, out_len, out) != out_len)
        {
            perror("Fwrite() failed");
            exit(1);
        }

        fprintf(stderr, "%lld -> %lld\r", _ftelli64(in), _ftelli64(out));
    }

    delete ulz;

    delete[] in_buf;
    delete[] out_buf;

    fclose(in);
    fclose(out);

    return 0;
}
