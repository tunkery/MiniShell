#include "../minishell.h"

void *ft_realloc(void *ptr, size_t size)
{
    void *new_ptr;

    if (ptr == NULL)
        return malloc(size);
    if (size == 0)
    {
        free(ptr);
        return NULL;
    }
    new_ptr = malloc(size);
    if (new_ptr == NULL)
        return NULL;
    ft_memcpy(new_ptr, ptr, size);
    free(ptr);
    return new_ptr;
}



size_t strspn(const char *s, const char *accept)
{
    size_t n = 0;

    while (*s && ft_strchr(accept, *s++))
        n++;
    return n;
}

size_t strcspn(const char *s, const char *reject)
{
    size_t i = 0;

    while (s[i] && !ft_strchr(reject, s[i]))
        i++;
    return i;
}

char *strtok(char *str, const char *delim)
{
    static char *last;
    char *start;
    char *end;

    if (str == NULL)
        str = last;

    if (str == NULL)
        return NULL;

    start = str + strspn(str, delim);
    if (*start == '\0')
        return NULL;

    end = start + strcspn(start, delim);
    if (*end != '\0')
        *end++ = '\0';

    last = end;
    return start;
}