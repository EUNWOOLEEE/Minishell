/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eunwolee <eunwolee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 12:20:49 by eunwolee          #+#    #+#             */
/*   Updated: 2023/08/11 16:43:53 by eunwolee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

t_bool			expand(t_data *data, t_token **token, int *i, t_bool quote);
static t_bool	check_heredoc(t_data *data, t_token *token, int *i);
static t_bool	check_blank(t_data *data, t_token *token, int *i, t_bool quote);
static t_bool	check_other(t_data *data, t_token *token, int *i, t_bool quote);
static void		replace(t_data *data, t_token *token, char *name);
static t_bool	check_char(char c);

t_bool	expand(t_data *data, t_token **token, int *i, t_bool quote)
{
	char	*name;
	char	*tmp;

	*i += 1;
	name = ft_strdup("");
	if (check_heredoc(data, *token, i) == TRUE)
		return (TRUE);
	if (check_blank(data, *token, i, quote) == TRUE)
		return (TRUE);
	if (check_other(data, *token, i, quote) == TRUE)
		return (TRUE);
	if (data->input[*i] == '?')
	{
		free(name);
		tmp = ft_itoa(data->error_code);
		(*token)->str = ft_strncat((*token)->str, tmp, ft_strlen(tmp));
		if (data->input[*i + 1] != ' ' && data->input[*i + 1] != '\t' && data->input[*i + 1] != '\0')
			(*token)->blank = FALSE;
		if (quote == TRUE && data->input[*i + 1] != '\"')
			*i += 1;
		else if ((quote == TRUE && data->input[*i + 1] == '\"'))
			*i += 2;
		return (FALSE);
	}
	while (!ft_isalnum(data->input[*i]) && data->input[*i] != '\0')
	{
		if (quote == FALSE && (data->input[*i] == ' ' || data->input[*i] == '\t'))
			break ;
		(*token)->print = TRUE;
		name = ft_strncat(name, &data->input[*i], 1);
		if (!name)
			program_error_exit("bash");
		*i += 1;
	}
	if (data->input[*i] == '\0')
		(*token)->blank = FALSE;
	while (check_char(data->input[*i]) == TRUE)
	{
		name = ft_strncat(name, &data->input[*i], 1);
		if (!name)
			program_error_exit("bash");
		*i += 1;
	}
	replace(data, *token, name);
	if (quote == TRUE && data->input[*i] == '\"')
	{
		if (data->input[*i + 1] == '\0')
			return (TRUE);
		(*token)->blank = FALSE;
		token_add_list(&data->tokens, token, TRUE);
		*i += 1;
	}
	else if (quote == FALSE)
	{
		if ((data->input[*i] != ' ' && data->input[*i] != '\t' && data->input[*i] != '\0'))
			(*token)->blank = FALSE;
		*i -= 1;
		token_add_list(&data->tokens, token, TRUE);
	}
	return (FALSE);
}

static t_bool	check_char(char c)
{
	//c == '\0' 아니고?
	if (c != '\'' \
		&& c != '\"' \
		&& c != ' ' \
		&& c != '\t' \
		&& c != '\0' \
		&& ft_isalnum(c))
		return (TRUE);
	return (FALSE);
}

static t_bool	check_heredoc(t_data *data, t_token *token, int *i)
{
	t_list	*tmp;

	tmp = ft_lstlast(data->tokens);
	if (tmp && tmp->token->redirect_type == T_HEREDOC)
	{
		token->str = ft_strncat(token->str, "$", 1);
		if (!token->str)
			program_error_exit("bash");
		*i -= 1;
		return (TRUE);
	}
	return (FALSE);
}

static t_bool	check_blank(t_data *data, t_token *token, int *i, t_bool quote)
{
	if (quote == FALSE && (data->input[*i] == ' ' || data->input[*i] == '\t'))
	{
		token->str = ft_strncat(token->str, "$", 1);
		if (!token->str)
			program_error_exit("bash");
		if (quote == FALSE)
		{
			*i -= 1;
			return (TRUE);
		}
		token->str = ft_strncat(token->str, &data->input[*i], 1);
		if (!token->str)
			program_error_exit("bash");
		if (quote == TRUE)
			return (TRUE);
	}
	if (quote == TRUE && data->input[*i] == ' ')
		token->print = TRUE;
	return (FALSE);
}

static t_bool	check_other(t_data *data, t_token *token, int *i, t_bool quote)
{
	if (data->input[*i] == '\"')
	{

		if (quote == TRUE)
		{
			token->str = ft_strncat(token->str, "$", 1);
			if (!token->str)
				program_error_exit("bash");
		}
		else
		{
			if (data->input[*i + 1] == '\0')
				return (FALSE);
			*i -= 1;
		}
		return (TRUE);
	}
	else if (data->input[*i] == '\'')
	{
		*i -= 1 ;
		return (TRUE);
	}
	else if (data->input[*i] == '\0')
	{
		token->str = ft_strncat(token->str, "$", 1);
		if (!token->str)
			program_error_exit("bash");
		return (TRUE);
	}
	return (FALSE);
}

static void	replace(t_data *data, t_token *token, char *name)
{
	int		j;
	t_list	*tmp;

	tmp = env_search(data, name);
	if (!tmp)
	{
		token->blank = FALSE;
		token->str = ft_strncat(token->str, "$", 1);
		if (!token->str)
			program_error_exit("bash");
		token->str = ft_strncat(token->str, name, ft_strlen(name));
		if (!token->str)
			program_error_exit("bash");
	}
	else
	{
		j = 0;
		while (tmp->env[j] != '=')
			j++;
		token->print = TRUE;
		token->str = ft_strncat(token->str, \
			&tmp->env[j + 1], ft_strlen(&tmp->env[j + 1]));
		if (!token->str)
			program_error_exit("bash");
	}
	free(name);
}
