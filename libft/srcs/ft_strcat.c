void	ft_strcat(char *s1, const char *s2)
{
	int	idx;
    int s_idx;

	idx = 0;
    s_idx = -1;
	while(s1[idx])
        idx++;
    while(s2[++s_idx])
        s1[idx++] = s2[s_idx];    
    s1[idx] = '\0';    
}