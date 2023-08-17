#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_BACKGROUND 64

/* Splits the string by space and returns the array of tokens
 *
 */


int f1;
int f;
int forks;
int background[MAX_BACKGROUND];

char **tokenize(char *line)
{
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

	for (i = 0; i < strlen(line); i++)
	{

		char readChar = line[i];

		if (readChar == ' ' || readChar == '\n' || readChar == '\t')
		{
			token[tokenIndex] = '\0';
			if (tokenIndex != 0)
			{
				tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0;
			}
		}
		else
		{
			if (readChar == '&')
			{
				break;
			}
			token[tokenIndex++] = readChar;
		}
	}

	free(token);
	tokens[tokenNo] = NULL;
	return tokens;
}
void handle_signal_child(int signal)
{
	sleep(1);
	while (waitpid(-1, NULL, WNOHANG) > 0);
}
void handle_signal_child1(int signal)
{
	kill(f1, SIGKILL);
}
void ctrl_handler(int signal)
{
	if(f!=0)
	{
		kill(f, SIGKILL);
		printf("\n");
	}
	else
	{
		printf("\n");
	}
}

int main()
{
	signal(SIGINT, ctrl_handler);
	signal(SIGUSR1, handle_signal_child);
	signal(SIGUSR2, handle_signal_child1);
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i;
	while (1)
	{

		

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		// printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; // terminate with new line
		tokens = tokenize(line);

		

		// do whatever you want with the commands, here we just print them

		// shell implementation

		if (tokens[0] == NULL)
			continue;

		if (strcmp(tokens[0], "exit") == 0)
		{
			
			for (int k = 0; k < 64; k++)
			{
				
				if (background[k] != 0)
				{
					kill(background[k], SIGUSR2);
				}
				else
				{
					break;
				}
			}
			exit(0);
		}
		
		char path[200];
		if(strcmp(tokens[0],"cd")==0)
		{
			if(strlen(line)==3)
			{
				printf("Incorrect command\n");
			}
			else if(strcmp(tokens[1],"..")==0)
			{
				chdir("..");
				printf("%s",getcwd(path,200));
			}
			else
			{
				if(chdir(tokens[1])==-1)
				{
					printf("Incorrect command\n");
				}
				else
			    {
					chdir(tokens[1]);
				    printf("%s",getcwd(path,200));
			    }
			}
			continue;
		}





		f = fork();
		
		if (f < 0)
		{
			printf("Unable to create child process\n");
		}
		else if (f == 0)
		{
			setpgrp();
			if (line[strlen(line)-2] == '&')
			{

				f1 = fork();
				if (f1 < 0)
				{
					printf("Can't handle background process\n");
				}
				else if (f1 == 0)
				{

					execvp(tokens[0], tokens);
					printf("Command failed \n");
					exit(0);
				}
				else
				{
					int w = waitpid(f1,NULL,0);
					printf("Shell: Background process finished\n");
					kill(getppid(), SIGUSR1);
					exit(0);
				}
			}
			else
			{
				execvp(tokens[0], tokens);
				printf("Command failed \n");
				exit(1);
			}
		}
		else
		{
			if (line[strlen(line)-2] == '&')
			{
				background[forks++] = f;
			}else
			{
				int wc = waitpid(f,NULL,0);
			}

		
		}

		/*for(i=0;tokens[i]!=NULL;i++){
				printf("found token %s (remove this debug output later)\n", tokens[i]);
		}*/
		// Freeing the allocated memory
		for (i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}
