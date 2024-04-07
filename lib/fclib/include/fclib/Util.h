#include <string.h>
#include <cstddef>
#include <stdlib.h>
#ifndef FCLIB_UTIL_H
#define FCLIB_UTIL_H

namespace FCLIB
{

    namespace Util
    {

        bool startsWith(const char *var, const char *prefix);

        bool equal(const char *s1, const char *s2);
        bool equalIgnoreCase(const char *s1, const char *s2);

        bool equalAny(const char *s1, const char *match1, const char *match2 = NULL, const char *match3 = NULL, const char *match4 = NULL);

        bool isEmpty(const char *s);

        bool endsWith(const char *s, const char *end);
        int toMsecs(const char *s);

        // text in format "text1:int1,text2:int2,..."
        // for example "repeat:1,stretch:2,"clip:3" with text "repeat" returns 1
        int mapText2Int(const char *text, const char *val, int defaultValue);

        bool isNameChar(char c);

        char *allocText(const char *from);

        char *allocText(size_t length);
        void freeText(const char *text);
        void toUpperCase(char *text);
        const char *next(const char *text, const char *substr);

        template <typename T, typename T1, typename T2>
        T clamp(T val, T1 min, T2 max)
        {
            if (val < min)
            {
                return min;
            }

            if (val > max)
            {
                return max;
            }
            return val;
        };

    }
}
#endif