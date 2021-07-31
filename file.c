#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
typedef struct s_format
{
    char spec;
    int wid;
    int prec;
} t_format;

t_format init_parse(void)
{
    t_format s;
    s.prec = 0;
    s.wid = 0;
    s.spec = ' ';
    return s;
}

int ft_strlen(char *str)
{
    int i = 0;

    if (str == NULL)
        return 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}


int ft_atoi(char *str)
{
    int i = 0;
  //  int sign = 1;
    long long int result = 0;

    if (str[i] == '-' || str[i] == '+')
    {
        i++;
    }
    while (is_digit(str[i]) == 1)
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

int write_char(char c)
{
    write(1, &c, 1);
    return 1;
}
int ft_count(long long n, char *base)
{
    int i = 0;
    if (n < 0)
        i++;
    while (n / ft_strlen(base) != 0)
    {
        i++;
        n = n / ft_strlen(base);
    }
    return (i + 1);
}
char *ft_itoa(long long nbr, char *base)
{
    char *str;
    int length;
    int tmp;
    length = ft_count(nbr, base);
    tmp = length;
    str = malloc(sizeof(char) * (length + 1));
    if (str != NULL)
    {
        if (nbr < 0)
        {
            str[0] = '-';
            nbr = nbr * -1;
        }
        while (nbr / ft_strlen(base) != 0 && length >= 0)
        {
            str[length - 1] = base[nbr % ft_strlen(base)];
            nbr = nbr / ft_strlen(base);
            length--;
        }
        str[length - 1] = base[nbr];
        str[tmp] = '\0';
    }
    return (str);
}

int write_string(char *str, int len)
{
    int i = 0;
    while (i < len)
    {
        write(1, &str[i], 1);
        i++;
    }
    return i;
}

char *ft_substr(char *str, int start, int len)
{
    char *ptr;
    int i = 0;

    ptr = malloc(sizeof(char *) * (ft_strlen(str) + 1));
    if (ptr == NULL)
        return NULL;
    if (start >= ft_strlen(str))
        return NULL;
    while (i < len)
    {
        ptr[i] = str[start];
        i++;
        start++;
    }
    ptr[i] = '\0';
    return ptr;
}

void parse_wid(int *i, char *arg, t_format *parser)
{
    char *result;
    int count = 0;
    if (arg[*i] == '\0')
       return;
    if(arg[*i] == '-')
        *i = *i + 1;
    while (is_digit(arg[*i]) == 1)
    {
        count++;
        *i = *i + 1;
    }
    if (count == 0)
        return;
    result = ft_substr(arg, *i - count, count);
    parser->wid = ft_atoi(result);
    free(result);
}

void parse_prec(int *i, char *arg, t_format *parser)
{
    char *result;
    int count = 0;
    if (arg[*i] == '\0' || arg[*i] != '.')
        return;
    *i = *i + 1;
    if (is_digit(arg[*i]) == 1)
    {
        while (is_digit(arg[*i]) == 1)
        {
            count++;
            *i = *i + 1;
        }
        if (count == 0)
            return;
        result = ft_substr(arg, *i - count, count);
        parser->prec = ft_atoi(result);
        free(result);
    }
}
void parse_spec(int *i, char *arg, t_format *parser)
{
    if (arg[*i] == '\0')
        return;
    if (arg[*i] == 's' || arg[*i] == 'x' || arg[*i] == 'd')
    {
        parser->spec = arg[*i];
        *i = *i + 1;
    }
}
int output_d(va_list *list, t_format *parser)
{
    char *arg;
    long long d;
    int prec = parser->prec;
    int wid = parser->wid;
    int count = 0;
    int tmp;
    d = va_arg(*list, int);
    tmp = d;

    if (d < 0)
        d = d * -1;
    arg = ft_itoa(d, "0123456789");
    if (prec <= ft_strlen(arg))
        prec = 0;
    if (wid <= ft_strlen(arg))
        wid = 0;
    int len = wid - ft_strlen(arg);
    if (wid > ft_strlen(arg))
    {
        if (prec > ft_strlen(arg))
            len = wid - prec;
        if (tmp < 0)
            len = len - 1;
        while (len > 0)
        {
            count += write_char(' ');
            len--;
        }
        len = prec - ft_strlen(arg);
        if (tmp < 0)
            count += write_char('-');
        while (len > 0)
        {
            count += write_char('0');
            len--;
        }
    }
    if (prec > ft_strlen(arg) && wid < ft_strlen(arg))
    {
        len = prec - ft_strlen(arg);
        if (tmp < 0)
            count += write_char('-');
        while (len > 0)
        {
            count += write_char('0');
            len--;
        }
    }
    if (wid == 0 && prec == 0 && tmp < 0)
        count += write_char('-');
    count += write_string(arg, ft_strlen(arg));
    free(arg);
    return count;
}

int output_x(va_list *list, t_format *parser)
{
    char *arg;
    long long x;
    int prec = parser->prec;
    int wid = parser->wid;
    int count = 0;
    x = va_arg(*list, unsigned int);
    arg = ft_itoa(x, "0123456789abcdef");
    int len = wid - ft_strlen(arg);
    if (wid > ft_strlen(arg))
    {
        if (prec > ft_strlen(arg))
            len = wid - prec;
        while (len > 0)
        {
            count += write_char(' ');
            len--;
        }
        len = prec - ft_strlen(arg);
        while (len > 0)
        {
            count += write_char('0');
            len--;
        }
    }
    if (prec > ft_strlen(arg) && wid < ft_strlen(arg))
    {
        len = prec - ft_strlen(arg);
        while (len > 0)
        {
            count += write_char('0');
            len--;
        }
    }
    count += write_string(arg, ft_strlen(arg));
    free(arg);
    return count;
}

int output_s(va_list *list, t_format *parser)
{
    char *str;
    int wid = parser->wid;
    int prec = parser->prec;
    int count = 0;
    int len_wid;

    str = va_arg(*list, char *);
    if (str == NULL)
        str = "(null)";
    if (prec >= ft_strlen(str) || prec == 0)
        prec = ft_strlen(str);
    len_wid = wid - prec;
    if (wid > ft_strlen(str))
    {
        while (len_wid > 0)
        {
            count += write_char(' ');
            len_wid--;
        }
    }
    count = write_string(str, prec);
    return count;
}

int output(va_list *list, t_format *parser)
{
    if (parser->spec == ' ')
        return 0;
    if (parser->spec == 's')
        return output_s(list, parser);
    if (parser->spec == 'x')
        return output_x(list, parser);
    if (parser->spec == 'd')
        return output_d(list, parser);
    return 0;
}

int ft_printf(char *arg, ...)
{
    int count = 0;
    int i = 0;
    va_list list;
    t_format parser;

    va_start(list, arg);
    while (arg[i] != '\0')
    {
        if (arg[i] == '%')
        {
            parser = init_parse();
            i++;
            parse_wid(&i, arg, &parser);
            parse_prec(&i, arg, &parser);
            parse_spec(&i, arg, &parser);
            count += output(&list, &parser);
        }
        count += write_char(arg[i++]);
    }
    va_end(list);
    return count;
}

// int main()
// { printf("%d ", 0);
//     ft_printf("%d ", 0);

//     // printf("%.-5x\n", 2147483647);
//     // ft_printf("%.-5x\n", 2147483647);
//     // printf("%1.2x\n", -22);
//     // ft_printf("%1.2x\n", -22);

//     // printf("%d\n", -22);
//     // ft_printf("%d\n", -22);
//     // printf("%1.7d\n", -22);
//     // ft_printf("%1.7d\n", -22);
//     // printf("%.3d\n", -22);
//     // ft_printf("%.3d\n", -22);
//     // int i = ft_count(15, "0123456789");
//     // int k = ft_count(-155, "0123456789");
//     // printf("%d %d ", i ,k);

// }

int main()
{
  int i;
    printf("\n----------------------------\n");
    i = printf("%s|\n", "hello");
    printf("-%d-", i);
    i = printf("%.s|\n", "hello");
    printf("-%d-", i);
    i = printf("%10s|\n", "hello");
    printf("-%d-", i);
    i = printf("%.8s|\n", "hello");
    printf("-%d-", i);
    i = printf("%10.0s|\n", "hello");
    printf("-%d-", i);
    printf("\n----------------------------\n");
     i = ft_printf("%s|\n", "hello");
    ft_printf("-%d-", i);
    i = ft_printf("%.s|\n", "hello");
    ft_printf("-%d-", i);
    i = ft_printf("%10s|\n", "hello");
    ft_printf("-%d-", i);
    i = ft_printf("%.8s|\n", "hello");
    ft_printf("-%d-", i);
    i = ft_printf("%10.0s|\n", "hello");
    ft_printf("-%d-", i);

    printf("\n----------------------------\n");

    printf("%0d|\n", 12);
    printf("%.d|\n", 12);
    printf("%10d|\n", 12);
    printf("%10.4d|\n", -16);
    printf("%10.0d|\n", 12);
    ft_printf("%10.12d|\n", 12);

     ft_printf("%0d|\n", 12);
    ft_printf("%.d|\n", 12);
    ft_printf("%10d|\n", 12);
    ft_printf("%10.4d|\n", -16);
    ft_printf("%10.0d|\n", 12);
    ft_printf("%10.12d|\n", 12);


    printf("%0d|\n", -12);
    printf("%.d|\n", -12);
    printf("%10d|\n", -12);
    printf("%.4d|\n", -12);
    printf("%10.0d|\n", -12);
    printf("%10.12d|\n", -12);

    ft_printf("%0d|\n", -12);
    ft_printf("%.d|\n", -12);
    ft_printf("%10d|\n", -12);
    ft_printf("%.4d|\n", -12);
    ft_printf("%10.0d|\n", -12);
    ft_printf("%10.12d|\n", -12);

    printf("----------------------------\n");
    printf("%0x|\n", 12);
    printf("%.x|\n", 12);
    printf("%10x|\n", 12);
    printf("%.4x|\n", 12);
    printf("%10.0x|\n", 12);
    printf("%10.12x|\n", 12);


    ft_printf("%0x|\n", 12);
    ft_printf("%.x|\n", 12);
    ft_printf("%10x|\n", 12);
    ft_printf("%.4x|\n", 12);
    ft_printf("%10.0x|\n", 12);
    ft_printf("%10.12x|\n", 12);


printf("----------------------------\n");

    printf("%0x|\n", -12);
    printf("%.x|\n", -12);
    printf("%10x|\n", -12);
    printf("%.4x|\n", -12);
    printf("%10.0x|\n", -12);
    printf("%10.12x|\n", -12);


    ft_printf("%0x|\n", -12);
    ft_printf("%.x|\n", -12);
    ft_printf("%10x|\n", -12);
    ft_printf("%.4x|\n", -12);
    ft_printf("%10.0x|\n", -12);
    ft_printf("%10.12x|\n", -12);
	    char *str;
    str = NULL;
    printf("abc: %10.0s, 12%2.0d, %10.0x\n", str, -12 ,53543);
    ft_printf("abc: %10.0s, 12%2.0d, %10.0x\n", str, -12, 53543);
printf("----------------------------\n");
printf("%7s%-5s\n", "hello", "world");
ft_printf("%7s%-5s\n", "hello", "world");
printf("----------------------------\n");
printf("|%.03s|\n", NULL);

ft_printf("|%.03s|\n", NULL);
printf("|%3.1s|\n", NULL);
ft_printf("|%3.1s|\n", NULL);
printf("|%9.1s|\n", NULL);
ft_printf("|%9.1s|\n", NULL);
printf("|%.0d|\n", 0);
ft_printf("|%.0d|\n", 0);
printf("|%.d|\n", 0);
ft_printf("|%.d|\n", 0);
printf("|%5.0d|\n", 0);
ft_printf("|%5.0d|\n", 0);
printf("|%5.d|\n", 0);
ft_printf("|%5.d|\n", 0);
printf("|%d|\n", INT_MIN);
ft_printf("|%d|\n", INT_MIN);
printf("|%d|\n", INT_MAX + 1);
ft_printf("|%d|\n", INT_MAX + 1);
printf("|%3.6s|\n", NULL);
ft_printf("|%3.6s|\n", NULL);
printf("|%20.6s|\n", NULL);
ft_printf("|%20.6s|\n", NULL);
printf("|%x|\n", 4294967295u);
ft_printf("|%x|\n", 4294967295u);
printf("Il fait au moins %x\n", -8000);
ft_printf("Il fait au moins %x\n", -8000);
printf("%x\n", INT_MIN);
ft_printf("%x\n", INT_MIN);
printf("%x\n", INT_MAX + 1);
ft_printf("%x\n", INT_MAX + 1);
i = printf("|%%|\n");
printf("%d\n", i);
i = ft_printf("|%%|\n");
printf("%d", i);
   return 0;
}
