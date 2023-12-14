int ft_strarrlen(char **arr)
{
    int len;

    len = 0;
    if (!arr)
        return (0);
    while (arr[len])
        len++;
    return (len);
}
