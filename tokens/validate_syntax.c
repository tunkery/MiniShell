/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:38:08 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/13 18:38:10 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_syntax_message(char *str, t_env *env)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	write(2, str, ft_strlen(str));
	write(2, " \n", 2);
	env->exit_code = 258;
	return (1);
}

int	validate_redirect_syntax(t_token *tokens, t_env *env)
{
	t_token	*tmp;

	tmp = tokens;
	env->exit_code = 0;
	while (tmp)
	{
		if (tmp->type == TOKEN_REDIRECT_IN || tmp->type == TOKEN_REDIRECT_OUT
			|| tmp->type == TOKEN_REDIRECT_APPEND || tmp->type == TOKEN_HEREDOC)
		{
			if (!tmp->next)
			{
				print_syntax_message("newline'", env);
				return (0);
			}
			if (tmp->next->type != TOKEN_WORD)
			{
				print_syntax_message(tmp->next->value, env);
				return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int	validate_pipe_syntax(t_token *tokens, t_env *env, int check)
{
	t_token	*tmp;

	tmp = tokens;
	env->exit_code = 0;
	if (tmp && tmp->type == TOKEN_PIPE)
		check = print_syntax_message("|'", env);
	if (check == 1)
		return (0);
	while (tmp)
	{
		if (tmp->type == TOKEN_PIPE)
		{
			if (!tmp->next)
				check = print_syntax_message("|'", env);
			if (tmp->next->type == TOKEN_PIPE)
				check = print_syntax_message("|'", env);
			if (tmp->prev && tmp->prev->type == TOKEN_PIPE)
				check = print_syntax_message("|'", env);
			if (check == 1)
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	validate_syntax(t_token *tokens, t_env *env)
{
	if (!tokens)
		return (1);
	if (!validate_redirect_syntax(tokens, env))
	{
		env->exit_code = 2;
		return (0);
	}
	if (!validate_pipe_syntax(tokens, env, 0))
	{
		env->exit_code = 2;
		return (0);
	}
	return (1);
}

// void print_syntax_message(char *str, t_env *env)
// {
//     write(2, "minishell: syntax error near unexpected token `", 47);
//     write(2, str, ft_strlen(str));
//     write(2, " \n", 2);
//     env->exit_code = 258;
// }

// int validate_redirect_syntax(t_token *tokens, t_env *env)
// {

//     t_token *tmp = tokens;
//     env->exit_code = 0;

//     while(tmp)
//     {
//         if(tmp->type == TOKEN_REDIRECT_IN || tmp->type == TOKEN_REDIRECT_OUT
//            tmp->type == TOKEN_REDIRECT_APPEND
//		|| tmp->type == TOKEN_HEREDOC)
//         {

//             // Redirect token'dan sonra bir token yoksa sözdizimi hatası
//             if(!tmp->next)
//             {

//                 print_syntax_message("newline'", env);
//                 env->exit_code = 258;
//                 return (0);
//             }

//
// Redirect token'dan sonraki token WORD değilse sözdizimi hatası
//             if(tmp->next->type != TOKEN_WORD)
//             {

//                 print_syntax_message(tmp->next->value, env);
//                 env->exit_code = 258;
//                 return (0);
//             }

//             // Heredoc için özel kontrol - boş delimiter geçersizdir
//             if(tmp->type == TOKEN_HEREDOC)
//             {

//                 if(!tmp->next->value || tmp->next->value[0] == '\0')
//                 {

//                     print_syntax_message("newline'", env);
//                     env->exit_code = 258;
//                     return (0);
//                 }
//             }
//         }
//         tmp = tmp->next;
//     }

//     return (1);
// }

// int validate_pipe_syntax(t_token *tokens, t_env *env)
// {
//     t_token *tmp = tokens;
//     env->exit_code = 0;
//     // For only one pipe!
//     if(tmp && tmp->type == TOKEN_PIPE)
//     {
//         print_syntax_message("|'", env);
//         return (0);
//     }
//     while(tmp)
//     {
//         if(tmp->type == TOKEN_PIPE)
//         {
//             if(!tmp->next)
//             {
//                 print_syntax_message("|'", env);
//                 return (0);
//             }
//             if(tmp->next->type == TOKEN_PIPE)
//             {
//                 print_syntax_message("|'", env);
//                 return (0);
//             }
//             if(tmp->prev && tmp->prev->type == TOKEN_PIPE)
//             {
//                 print_syntax_message("|'", env);
//                 return (0);
//             }
//         }
//         tmp = tmp->next;
//     }
//     return (1);
// }

// int validate_syntax(t_token *tokens, t_env *env)
// {
//     if(!tokens)
//     return (1);
//     if(!validate_redirect_syntax(tokens, env))
//     {
//         env->exit_code = 2;

//         return (0);
//     }
//     if(!validate_pipe_syntax(tokens,env))
//     {
//         env->exit_code = 2;
//         return (0);
//     }
//     return (1);
// }
