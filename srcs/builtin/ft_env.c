/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichan <kichan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 19:15:04 by kichlee           #+#    #+#             */
/*   Updated: 2023/08/06 23:28:27 by kichan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"
void    ft_env(t_data *data, t_leaf *cur_root);

char	*check_dollar_two(t_data *data, char *str)
{
	t_list *tmp;
	tmp = env_search(data, str);
	if(tmp == NULL)
		return (NULL);
	else
		return (tmp->env);
}
/**
 *  1. cmd cnt 를 해주고 cmd가 1개 인경우는
 *      -> env_print 실행
 *  2. cmd cnt 가 1개가 아닐때
 *      -> $가 있을때
 *          $뒤 path가 존재 할때 $뒤를 검색한 다음에 해당하는 path만 반환
 *          $뒤 path가 존재 없을때  env_print 실행
 *      -> $가 없을때 해당 문자열
 *           env: 문자열: No such file or directory
 *
 * 
 *      export $kichlee 있는 거면  kichlee
 *      export $aaa 없는 거면 $aaaa
*/

void    ft_env(t_data *data, t_leaf *cur_root)
{
    char    **cmd;

    cmd = join_cmd(cur_root->left_child->right_child);
    
    if(!cmd[1])
        env_print(data);
    else
    {
        if(cmd[1][0] == '$')
        {
            if(ft_strlen(cmd[1]) == 1)
                printf("env: %s: No such file or directory\n", cmd[1]);
            else if(check_dollar_two(data, cmd[1]) == NULL)
                env_print(data);
            else
                printf("%s", env_search(data, cmd[1])->env);
        }
        else
            printf("env: %s: No such file or directory\n", cmd[1]);
    }
    // return (TRUE);
}
