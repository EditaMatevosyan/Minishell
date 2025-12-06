#include "minishell.h"

int	count_heredocs(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && tok->type != T_PIPE)
	{
		if (tok->type == T_HEREDOC)
			count++;
		tok = tok->next;
	}
	return (count);
}