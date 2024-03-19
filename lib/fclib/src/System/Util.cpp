#include <string.h>
#include <cstddef>
#include <stdlib.h>
#include <ctype.h>

namespace FCLIB
{

    namespace Util
    {
        /* chars that can be in names in addition to alpha-numerics*/
        const char *NAME_CHARS = "-_.#$%";

        bool startsWith(const char *var, const char *prefix)
        {
            if (var == NULL)
            {
                return false;
            }
            if (prefix == NULL || prefix[0] == 0)
            {
                return true;
            }
            return strncmp(var, prefix, strlen(prefix)) == 0;
        };

        bool equal(const char *s1, const char *s2)
        {
            if (s1 == s2)
            {
                return true;
            }
            if ((s1 == NULL && s2 != NULL) || (s1 != NULL && s2 == NULL))
            {
                return false;
            }
            return strcmp(s1, s2) == 0;
        }

        bool equalAny(const char *s1, const char *match1, const char *match2 = NULL, const char *match3 = NULL, const char *match4 = NULL)
        {
            return Util::equal(s1, match1) || Util::equal(s1, match2);
        }

        bool isEmpty(const char *s)
        {
            return s == NULL || s[0] == 0;
        }

        bool endsWith(const char *s, const char *end)
        {
            if (s == NULL || end == NULL)
            {
                return false;
            }
            int el = strlen(end);
            int sl = strlen(s);
            if (el > sl)
            {
                return false;
            }
            return strcmp(s + sl - el, end) == 0;
        }
        int toMsecs(const char *s)
        {
            int val = 0;
            if (s == NULL || !isdigit(*s))
            {
                return val;
            }
            if (Util::endsWith(s, "ms") || Util::endsWith(s, "msec"))
            {
                val = atoi(s);
            }
            else if (Util::endsWith(s, "s") || Util::endsWith(s, "sec"))
            {
                val = atoi(s) * 1000;
            }
            else if (Util::endsWith(s, "m") || Util::endsWith(s, "min"))
            {
                val = atoi(s) * 1000 * 60;
            }
            else if (Util::endsWith(s, "h") || Util::endsWith(s, "hr") || Util::endsWith(s, "hour"))
            {
                val = atoi(s) * 1000 * 60 * 60;
            }
            else
            {
                val = atoi(s);
            }
            return val;
        }

        // text in format "text1:int1,text2:int2,..."
        // for example "repeat:1,stretch:2,"clip:3" with text "repeat" returns 1
        int mapText2Int(const char *text, const char *val, int defaultValue)
        {
            if (text == NULL || val == NULL)
            {
                return defaultValue;
            }
            const char *pos = text;
            int len = strlen(val);
            while (*pos != 0 && (strncmp(pos, val, len) != 0 || pos[len] != ':'))
            {
                pos += 1;
            }
            if (pos[len] == ':')
            {
                return atoi(pos + len + 1);
            }
            else
            {
                return defaultValue;
            }
        }

        bool isNameChar(char c)
        {
            return isalnum(c) || strchr(NAME_CHARS, c) != NULL;
        }

        void toUpperCase(char *text)
        {
            while (*text != 0)
            {
                *text = toupper(*text);
                text++;
            }
        }

        char *allocText(const char *from)
        {
            int len = 0;
            if (from != NULL && from[0] != 0)
            {
                len = strlen(from);
            }
            char *text = (char *)malloc(len + 1);
            strcpy(text, from);
            return text;
        }

        char *allocText(size_t length)
        {
            char *text = (char *)malloc(length + 1);
            memset(text, 0, length + 1);
            return text;
        }

        void freeText(const char *text)
        {
            if (text != NULL)
            {
                free((void *)text);
            }
        }

        const char *next(const char *text, const char *substr)
        {
            if (text == NULL || substr == NULL)
            {
                return NULL;
            }
            const char *pos = text;
            size_t len = strlen(substr);
            while (pos[0] != 0 && strncmp(pos, substr, len) != 0)
            {
                pos++;
            }
            if (pos[0] == 0)
            {
                return NULL;
            }
            return pos;
        }

    }
}
