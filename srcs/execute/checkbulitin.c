#include "../../incs/minishell.h"

int	check_bulitin(t_data *data)
{
	int		bulit;
	t_leaf	*temp;

	temp = data->root->left_child->right_child;
	if (temp != NULL)
		bulit = check_bulitin_func(temp->token->str);
	if (temp == NULL)
	{
		check_redirect(data->root, data);
		recover_std(data);
		return (TRUE);
	}
	if (bulit != 0)
	{
		check_redirect(data->root, data);
		exec_bulitin(bulit, data);
		recover_std(data);
		return (TRUE);
	}
	return (FALSE);
}
