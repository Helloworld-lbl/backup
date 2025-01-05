#include <lz77/ulz.h>

template <typename T>
inline T ULZ::Min(T a, T b)
{
    return (a < b) ? a : b;
}

template <typename T>
inline T ULZ::Max(T a, T b)
{
    return (a > b) ? a : b;
}

inline U16 ULZ::UnalignedLoad16(void *p)
{
    return *reinterpret_cast<const U16 *>(p);
}

inline U32 ULZ::UnalignedLoad32(void *p)
{
    return *reinterpret_cast<const U32 *>(p);
}

inline void ULZ::UnalignedStore16(void *p, U16 x)
{
    *reinterpret_cast<U16 *>(p) = x;
}

inline void ULZ::UnalignedCopy32(void *d, void *s)
{
    *reinterpret_cast<U32 *>(d) = UnalignedLoad32(s);
}

inline void ULZ::WildCopy(U8 *d, U8 *s, int n)
{
    UnalignedCopy32(d, s);
    UnalignedCopy32(d + 4, s + 4);

    for (int i = 8; i < n; i += 8)
    {
        UnalignedCopy32(d + i, s + i);
        UnalignedCopy32(d + i + 4, s + i + 4);
    }
}

inline U32 ULZ::Hash32(void *p)
{
    return (UnalignedLoad32(p) * 0x9E3779B9) >> (32 - HASH_BITS);
}

inline void ULZ::EncodeMod(U8 *&p, U32 x)
{
    while (x >= 128)
    {
        x -= 128;
        *p++ = 128 + (x & 127);
        x >>= 7;
    }
    *p++ = x;
}

inline U32 ULZ::DecodeMod(U8 *&p)
{
    U32 x = 0;
    for (int i = 0; i <= 28; i += 7)
    {
        const int c = *p++;
        x += c << i;
        if (c < 128)
            break;
    }
    return x;
}

// LZ77

int ULZ::Compress(U8 *in, int in_len, U8 *out, int level)
{
    const int max_chain = (level < 8) ? 1 << level : WINDOW_SIZE;
    U8 *op = out;

    for (int i = 0; i < HASH_SIZE; ++i)
        Head[i] = NIL;

    int run = 0;

    int p = 0;
    while (p < in_len)
    {
        int best_len = MIN_MATCH - 1;
        int dist = 0;

        const int max_match = in_len - p;
        if (max_match >= MIN_MATCH)
        {
            int limit = Max(p - WINDOW_SIZE, NIL);
            int chain_len = max_chain;

            int s = Head[Hash32(&in[p])];
            while (s > limit)
            {
                if (in[s + best_len] == in[p + best_len] && UnalignedLoad32(&in[s]) == UnalignedLoad32(&in[p]))
                {
                    int len = MIN_MATCH;
                    while (len < max_match && in[s + len] == in[p + len])
                        ++len;

                    if (len > best_len)
                    {
                        best_len = len;
                        dist = p - s;

                        if (len == max_match)
                            break;
                    }
                }

                if (!--chain_len)
                    break;

                s = Tail[s & WINDOW_MASK];
            }

            if (best_len == MIN_MATCH && run >= (7 + 128))
                best_len = 0;

            if (level == 9 && best_len >= MIN_MATCH && best_len < max_match)
            {

                for (int i = 1; i <= 2 && best_len; ++i) // 2-byte lookahead
                {
                    const int target_len = best_len + i;
                    const int j = p + i;

                    limit = Max(j - WINDOW_SIZE, NIL);
                    chain_len = max_chain;

                    s = Head[Hash32(&in[j])];
                    while (s > limit)
                    {
                        if (in[s + best_len] == in[j + best_len] && UnalignedLoad32(&in[s]) == UnalignedLoad32(&in[j]))
                        {
                            int len = MIN_MATCH;
                            while (len < target_len && in[s + len] == in[j + len])
                                ++len;

                            if (len == target_len)
                            {
                                best_len = 0;
                                break;
                            }
                        }

                        if (!--chain_len)
                            break;

                        s = Tail[s & WINDOW_MASK];
                    }
                }
            }
        }

        if (best_len >= MIN_MATCH)
        {
            const int len = best_len - MIN_MATCH;
            const int tmp = ((dist >> 12) & 16) + Min(len, 15);

            if (run)
            {
                if (run >= 7)
                {
                    *op++ = (7 << 5) + tmp;
                    EncodeMod(op, run - 7);
                }
                else
                    *op++ = (run << 5) + tmp;

                WildCopy(op, &in[p - run], run);
                op += run;

                run = 0;
            }
            else
                *op++ = tmp;

            if (len >= 15)
                EncodeMod(op, len - 15);

            UnalignedStore16(op, dist);
            op += 2;

            while (best_len--)
            {
                const U32 h = Hash32(&in[p]);
                Tail[p & WINDOW_MASK] = Head[h];
                Head[h] = p++;
            }
        }
        else
        {
            ++run;

            const U32 h = Hash32(&in[p]);
            Tail[p & WINDOW_MASK] = Head[h];
            Head[h] = p++;
        }
    }

    if (run)
    {
        if (run >= 7)
        {
            *op++ = 7 << 5;
            EncodeMod(op, run - 7);
        }
        else
            *op++ = run << 5;

        WildCopy(op, &in[p - run], run);
        op += run;

        // run=0;
    }

    return op - out;
}

int ULZ::Decompress(U8 *in, int in_len, U8 *out, int out_len)
{
    U8 *op = out;
    U8 *ip = in;
    const U8 *ip_end = ip + in_len;
    const U8 *op_end = op + out_len;

    while (ip < ip_end)
    {
        const int tag = *ip++;
        if (tag >= 32)
        {
            int run = tag >> 5;
            if (run == 7)
                run += DecodeMod(ip);
            if ((op + run) > op_end) // Overrun check
                return -1;

            WildCopy(op, ip, run);
            op += run;
            ip += run;
            if (ip >= ip_end)
                break;
        }

        int len = (tag & 15) + MIN_MATCH;
        if (len == (15 + MIN_MATCH))
            len += DecodeMod(ip);
        if ((op + len) > op_end) // Overrun check
            return -1;

        const int dist = ((tag & 16) << 12) + UnalignedLoad16(ip);
        ip += 2;
        U8 *cp = op - dist;
        if (cp < out) // Range check
            return -1;

        if (dist >= 4)
        {
            WildCopy(op, cp, len);
            op += len;
        }
        else
        {
            while (len--)
                *op++ = *cp++;
        }
    }

    return (ip == ip_end) ? op - out : -1;
}
