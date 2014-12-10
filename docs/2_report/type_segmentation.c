struct Char
{
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    struct Char *next;
};

struct Line
{
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    struct Char *l_char;
    struct Line *next;
};

struct Bloc
{
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    struct Line *l_line;
    struct Bloc *next;
};