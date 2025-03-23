/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:08:19 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/23 12:06:29 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**update_env(char **envp, char *key)
{
	int i;
	int size;
	char **new_env;
	
	i = 0;
	size = env_size(envp) + 1;
	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	copy_env(envp, &new_env);
	new_env[size - 1] = ft_strdup(key);
	new_env[size] = NULL;
	DEBUG_PRINT(GRN"New env created with key: %s\n"RESET, key);
	clean_2d(envp); // freee old envp
	DEBUG_PRINT(GRN"Updated env with key: %s\n"RESET, key);
	return (new_env);
}

char	**remove_env(char **envp, char *key)
{
	int i;
	int j;
	int size;
	char **new_env;

	i = 0;
	j = 0;
	size = env_size(envp);
	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0
			&& (envp[i][ft_strlen(key)] == '=' || envp[i][ft_strlen(key)] == '\0'))
		{
			free(envp[i]);
			i++;
		}
		if (envp[i])
		{
			new_env[j] = ft_strdup(envp[i]);
			i++;
			j++;
		}
	}
	new_env[j] = NULL;
	clean_2d(envp); //  free ald envp
	DEBUG_PRINT(GRN"Removed env with key: %s\n"RESET, key);
	return (new_env);
}

void	initiate_env(t_env *env, char **envp)
{
	int i;

	i = 0;
	copy_env(envp, &(env->envp));
	copy_env(envp, &(env->export));
	if (!env->envp)
		return ;
	env->path = malloc(sizeof(char **) * 2);
	if (!env->path)
		return ;
	env->path1 = getenv("PATH");
}

// helper function that's needed for the find_exec

static char	*ft_gnls_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_s;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	i = 0;
	s_len = ft_gnl_strlen(s);
	if (start >= s_len)
		return (ft_gnl_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	new_s = (char *)malloc(len + 1);
	if (!new_s)
		return (NULL);
	while (i < len && s[start + i] != ':')
	{
		new_s[i] = s[start + i];
		i++;
	}
	new_s[i++] = '/';
	new_s[i] = '\0';
	return (new_s);
}

// finds the path to the executable we want to run

char	*find_exec(char *command, char *path, int i, int j)
{
	char *temp;

	if (!command || !path)
		return (NULL);
	while(path[i])
	{
		while (path[i] && path[i] && path[i] != ':')
			i++;
		temp = ft_gnls_substr(path, j, i - 4);
		if(!temp)
			return (NULL);
		temp = ft_strjoin(temp, command);
		if (access(temp, X_OK) == 0)
			return (temp);
		free(temp);
		temp = NULL;
		if(!path[i])
			break;
		j = i + 1;
		i++;
	}
	return (temp);
}

void exec_command(char **args, t_env *env, int out_fd)
{
	pid_t	pid;
	char *exec_path;

	if (!args || !args[0])
		return ;
	exec_path = find_exec(args[0], env->path1, 0, 5);
	if (!exec_path)
	{
		printf("minishell: %s: command not found.\n", args[0]);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		/*Added the openfile in here*/
		if(out_fd  != STDOUT_FILENO)
		{
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		if (execve(exec_path, args, env->path) == -1)
		{
			perror("execvp Failed");
			free(exec_path);
			exit(127);
		}
	}
	else if (pid < 0)
		perror("fork Failed");
	else
		wait(NULL);
	free(exec_path);

}
char **tokens_to_args(t_token *tokens)
{
    int count = 0;
    t_token *tmp = tokens;
    char **args;
    int i = 0;

    while(tmp && tmp->type != TOKEN_SEMIC)
    {
        if (tmp->type == TOKEN_WORD)
            count++;
        tmp = tmp->next;
    }

    args = malloc((count + 1) * sizeof(char *));
    if(!args)
        return NULL;
    tmp = tokens;
    while(tmp && tmp->type != TOKEN_SEMIC)
    {
        if(tmp->type == TOKEN_WORD)
        {
            args[i] = strdup(tmp->value); // change strdup to ft_strdup and test it again //TODO
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

// a function that runs the programs in the computer

void	cell_launch(t_token *tokens, t_env *env)
{
	t_token *tmp = tokens;
	char **args = NULL;
	int out_fd = STDOUT_FILENO;
	int save_stdout = dup(STDOUT_FILENO);
	char *heredoc_input = NULL;
	t_token *current;

	DEBUG_PRINT(BLUE"Starting Cell_lounch\n"RESET);
	while(tmp)
	{
		args = tokens_to_args(tmp);
		if(!args)
		{
			DEBUG_PRINT(RED"Failed to convert tokens to args\n"RESET);
			while(tmp && tmp->type != TOKEN_SEMIC)
				tmp = tmp->next;
			if(tmp)
				tmp = tmp->next;
			continue;
		}
		DEBUG_PRINT(BLUE"Args created\n"RESET);
		/* Check it all aguments in here*/
		int i = 0;
		while (args[i])
		{
			DEBUG_PRINT(BLUE"Args[%d]: %s\n"RESET, i, args[i]);
			i++;
		}
		current = tmp;
		while(current && current->type != TOKEN_SEMIC)
		{
			if (current->type == TOKEN_REDIRECT_APPEND) // >>
			{ 
				current = current->next;
				if(current && current->type == TOKEN_WORD)
				{
					out_fd = open(current->value, O_WRONLY | O_APPEND | O_CREAT, 0644); // 644 means you can read and write the file or directory and other users can only read it
					if(out_fd < 0)
					{
						perror("open failed!");
						clean_2d(args);
						return ;
					}
					DEBUG_PRINT(BLUE"Redirecting aoutput to '%s'\n"RESET, current->value);
				}
			}
			else if(current->type == TOKEN_HEREDOC) // <<
			{
				current = current->next;
				if(current && current->type == TOKEN_WORD)
				{
					heredoc_input = handler_heredoc(current->value);
					int pipe_fd[2];
					if(pipe(pipe_fd) == -1)
					{
						perror("pipe failed!");
						clean_2d(args);
						return ;
					} // Surecler arasi boruyu olusturur > alt surec icin
					write(pipe_fd[1], heredoc_input, ft_strlen(heredoc_input));
					close(pipe_fd[1]);
					dup2(pipe_fd[0], STDIN_FILENO);
					close(pipe_fd[0]);
					free(heredoc_input);
					DEBUG_PRINT(GRN"HEredoc process for delimiter '%s'\n"RESET, current->value);
				}
			}
			current = current->next;
		}
		if(builtin_check(args) != 0)
		{
			if(out_fd != STDOUT_FILENO)
			{
				if(dup2(out_fd, STDOUT_FILENO) == -1)
					perror("dup2 failed!");
				close(out_fd);
				DEBUG_PRINT(GRN"Output redirected to file\n"RESET);
			}
			run_builtin(args, env);
			if(out_fd != STDOUT_FILENO)
			{
				if(dup2(save_stdout, STDOUT_FILENO) == -1)
					perror("dup2 failed!");
				DEBUG_PRINT(GRN"STDOUT restored\n"RESET);
			}
		}
		else
		{
			exec_command(args, env, out_fd);
			DEBUG_PRINT(GRN"Command executed\n"RESET);
		}
		clean_2d(args);
		if(out_fd != STDOUT_FILENO)
			close(out_fd);
		out_fd = STDOUT_FILENO;
		while(tmp && tmp->type != TOKEN_SEMIC)
			tmp = tmp->next;
		if(tmp)
			tmp = tmp->next;
	}
	close(save_stdout);
	DEBUG_PRINT(BLUE"Ending Cell_lounch\n"RESET);
}
