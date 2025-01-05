#ifndef __ULZ_HPP
#define __ULZ_HPP

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

class ULZ
{
public:
    typedef unsigned char U8;
    typedef unsigned short U16;
    typedef unsigned int U32;

    static const int EXCESS = 16;

    static const int WINDOW_BITS = 17;
    static const int WINDOW_SIZE = 1 << WINDOW_BITS;
    static const int WINDOW_MASK = WINDOW_SIZE - 1;

    static const int MIN_MATCH = 4;

    static const int HASH_BITS = 18;
    static const int HASH_SIZE = 1 << HASH_BITS;
    static const int NIL = -1;

    // Hash chain

    int Head[HASH_SIZE];
    int Tail[WINDOW_SIZE];

    // Utils

    template <typename T>
    inline T Min(T a, T b);

    template <typename T>
    inline T Max(T a, T b);

    inline U16 UnalignedLoad16(void *p);

    inline U32 UnalignedLoad32(void *p);

    inline void UnalignedStore16(void *p, U16 x);

    inline void UnalignedCopy32(void *d, void *s);

    inline void WildCopy(U8 *d, U8 *s, int n);

    inline U32 Hash32(void *p);

    inline void EncodeMod(U8 *&p, U32 x);

    inline U32 DecodeMod(U8 *&p);

    // LZ77

    int Compress(U8 *in, int in_len, U8 *out, int level = 4);

    int Decompress(U8 *in, int in_len, U8 *out, int out_len);
};

#endif // __ULZ_HPP