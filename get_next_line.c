/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tunsal <tunsal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 19:09:23 by tunsal            #+#    #+#             */
/*   Updated: 2024/01/11 15:43:07 by tunsal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* Write `n` bytes of 0 into the memory starting from `s`. */
char	*ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*((unsigned char *) s + i) = (unsigned char) 0;
		++i;
	}
	return (s);
}

/* Returns 0 if `s` is NULL. */
static size_t	gnl_strlen(const char *s)
{
	int	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i] != '\0')
		++i;
	return (i);
}

/*
   Return joint string of s1 and s2. Free s1.
   return NULL upon alloc error.
*/
char	*gnl_joinfree(char const *s1, char const *s2)
{
	size_t	i;
	size_t	len1;
	size_t	len2;
	char	*joint;

	len1 = gnl_strlen(s1);
	len2 = gnl_strlen(s2);
	if (len1 == 0 && len2 == 0)
		return (NULL);
	joint = (char *) malloc((len1 + len2 + 1) * sizeof(char));
	if (joint == NULL)
	{
		free((char *) s1);
		return (NULL);
	}
	i = -1;
	while (++i < len1)
		joint[i] = s1[i];
	i = -1;
	while (++i < len2)
		joint[len1 + i] = s2[i];
	joint[len1 + len2] = '\0';
	free((char *) s1);
	return (joint);
}

static char	*extract_line(char *line, char *stash)
{
	char	*before_nl_section;
	char	*nl_char;
	int		err_flag;

	err_flag = 0;
	nl_char = ft_strchr(stash, '\n');
	if (nl_char)
	{
		before_nl_section = ft_substr(stash, 0, (nl_char - stash) + 1);
		line = gnl_joinfree(line, before_nl_section);
		free(before_nl_section);
		ft_memmove(stash, nl_char + 1, ft_strlen(nl_char + 1) + 1);
	}
	else
	{
		line = gnl_joinfree(line, stash);
		stash[0] = '\0';
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	stash[BUFFER_SIZE + 1];
	char		*line;
	int			read_count;
	int			err_flag;

	err_flag = 0;
	if (fd < 0 || BUFFER_SIZE < 1 || read(fd, stash, 0) < 0)
		return (ft_bzero(stash, BUFFER_SIZE + 1), NULL);
	line = NULL;
	read_count = BUFFER_SIZE;
	while (read_count == BUFFER_SIZE && !ft_strchr(stash, '\n'))
	{
		line = gnl_joinfree(line, stash);
		read_count = read(fd, stash, BUFFER_SIZE);
		if (read_count == -1)
			return (ft_bzero(stash, BUFFER_SIZE + 1), NULL);
		stash[read_count] = '\0';
	}
	return (extract_line(line, stash));
}
