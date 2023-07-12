/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eunwolee <eunwolee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:06:49 by eunwolee          #+#    #+#             */
/*   Updated: 2023/07/12 16:06:09 by eunwolee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

//토큰 출력용 임시 함수
void print_token(t_data *data)
{
	t_list *tmp = data->tokens;
	while (tmp)
	{
		printf("token: %d, redirect: %d, str: %s\n", tmp->token->type, tmp->token->redirect_type, tmp->token->str);
		tmp = tmp->next;
	}
}

int main(int argc, char **argv, char **envp)
{
	t_data	*data;
	
	(void)argc;
	(void)argv;
	init(&data, envp);

	char *str = get_next_line(0);
	data->input = ft_substr(str, 0, ft_strlen(str) - 1); //-1 개행 떼기
	lexer(data);
	syntax(data);
	parser(data);

	// env_print(data);
	// env_array_print(data);
	print_token(data);
	printf("\n\n");
	tree_print(data->root);
	
	return (0);
}