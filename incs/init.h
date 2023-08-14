/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eunwolee <eunwolee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 10:29:44 by eunwolee          #+#    #+#             */
/*   Updated: 2023/08/14 18:30:13 by eunwolee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# include "struct.h"

/*init*/
void	init(t_data **data, char **envp);
void	env_init(t_data *data, char **envp);
void	pipe_init(t_pipe **pipe);
void	info_init(t_info **info);

#endif