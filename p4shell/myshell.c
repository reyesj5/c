#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

char error_message[30] = "An error has occurred\n";
pid_t forkret = 0;
int bash = 0;

void myPrint(char *msg)
{
  write(STDOUT_FILENO, msg, strlen(msg));
}

int blankLine(char *line)
{
  char *ch;
  int is_blank = -1;
  for (ch = line; *ch != '\0'; ++ch)
  {
    if (!isspace(*ch))
    {
      is_blank = 0;
      break;
    }
  }
  return is_blank;
}

void freeArgs(char **args) {
  if (args == NULL) {
    return;
  }
  int i = 0;
  while (args++) {
    free(args[i]);
  }
  free(args);
}
  
char* getPath()
{
  int size = 1024;
  char cwd[size];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    return getcwd(cwd, sizeof(cwd));
  } else {
    return getcwd(cwd, sizeof(cwd)*4);
  }
}
  
char* parentDirectory()
{
  char *currPath = getPath();
  int i = strlen(currPath) - 1;
  while (i) {
    if (currPath[i] == '/') {
      currPath[i] = 0;
      i = 1;
    }
    i--;
  }
  return currPath;
}

char **parseArgs(char *args, char *com, char *delim) {
  char **argv = (char**)malloc(sizeof(char*)*1000);
  char *token;
  argv[0] = malloc(strlen(com) + 1);
  strncpy(argv[0], com, strlen(com));
  token = strtok(args, delim);
  int i = 0;
  while (token != NULL) {
    argv[i+1] = malloc(strlen(token) + 1);
    strncpy(argv[i+1], token, strlen(token));
    i++;
    token = strtok(NULL, delim);
  } 
  argv[i+1] = NULL;
  return argv;
}

void eliminateSpaces(char *str) {
  char* i = str;
  char* j = str;
  while(*j != 0)
    {
      *i = *j++;
      if(*i != ' ') {
	i++;
      }
    }
  *i = 0;
}

void removeTabs(char *str) {
  char* i = str;
  char* j = str;
  while(*j != 0)
    {
      *i = *j++;
      if(*i != '\t') {
	i++;
      }
    }
  *i = 0;
}

void parseCommand(char *command, char* args, int redir, int adv, char *output)
{
  eliminateSpaces(command);
  removeTabs(command);
  if (strlen(command) < 1) {
    return;
  }
  if (redir && !output[0]) {
    write(STDOUT_FILENO, error_message, strlen(error_message));
  }
  if (strcmp(command, "exit") == 0) {
    exit(0);
  } else if (strcmp(command, "pwd") == 0) {
    if (args[0]) {
      write(STDOUT_FILENO, error_message, strlen(error_message));
      exit(0);
    }
    char *path;
    path = getPath();
    myPrint(strcat(path, "\n"));
  } else if (strcmp(command, "cd") == 0) {
    char **argums = parseArgs(args, command, "/");
    int i = 1;
    if (!argums[i]) {
      int c = chdir(getenv("ROOT"));
      if (c < 0) {
	write(STDOUT_FILENO, error_message, strlen(error_message));
	return;
      }
    } else if (args[0] == '/' && (args[1] == ' ' || strlen(args) == 0)) {
      int c = chdir(getenv("ROOT"));
      if (c < 0) {
	write(STDOUT_FILENO, error_message, strlen(error_message));
	return;
      }
    } else if (strcmp(args, ".") == 0) {
      return;
    } else if (strcmp(argums[i], "..") == 0) {
      while (argums[i]) {
	int c = chdir("..");
	if (c < 0) {
	  write(STDOUT_FILENO, error_message, strlen(error_message));
	  return;
	}
	i++;
      }
    } else if (args[0] == '/' && strlen(args) > 1) {
      int c = chdir(args);
      if (c < 0) {
	write(STDOUT_FILENO, error_message, strlen(error_message));
	return;
      }
    } else {
      char *path = getPath();
      if (args[0] == '/') {
	strcat(path, args);
	int c = chdir(path);
	if (c < 0) {
	  write(STDOUT_FILENO, error_message, strlen(error_message));
	  return;
	}
      } else {
	strcat(path, "/");
	strcat(path, args);
	int c = chdir(path);
	if (c < 0) {
	  write(STDOUT_FILENO, error_message, strlen(error_message));
	  return;
	}
      }
    }
  } else {
    int status;
    if((forkret = fork()) == 0) {
      char **arguments = parseArgs(args, command, " ");
      char *data;
      int len;
      int fd = 0;
      if (redir && adv) {
	fd = open(output, O_RDWR);
	len = lseek(fd, 0, SEEK_END);
	data = (char*)malloc(sizeof(char)*len);
	int r = read(fd, data, len);
	if (r<0) {
	  write(STDOUT_FILENO, error_message, strlen(error_message));
	}
	dup2(fd, STDOUT_FILENO);
      } else if (redir) {
	if (!output[0]) {
	  write(STDOUT_FILENO, error_message, strlen(error_message));
	  kill(getpid(), 0);
	}
	fd = open(output, O_EXCL | O_CREAT);
	if (fd < 0) {
	  write(STDOUT_FILENO, error_message, strlen(error_message));
	  kill(getpid(), 0);
	}
	dup2(fd, STDOUT_FILENO);
      }
      int e = execvp(command, arguments);
      if (e == -1) {
	myPrint(command);
	write(STDOUT_FILENO, error_message, strlen(error_message));
	kill(getpid(), 0);
      }
      if (fd>2) {
	close(fd);
      }
      if (redir && adv) {
	fd = open(output, O_APPEND);
	int w = write(fd, data, len);
	if (w<0) {
	  write(STDOUT_FILENO, error_message, strlen(error_message));
	  kill(getpid(), 0);
	}
      }
      freeArgs(arguments);
      free(data);
      exit(0);
    } else {
      waitpid(forkret, &status, 0);
      wait(&status);
    }
  }
}

int main(int argc, char *argv[]) 
{
  char cmd_buff[514];
  char *pinput;
  bash = 0;
  FILE* bashFile;
  if (argc == 2) {
    bash = 1;
    bashFile = fopen(argv[1], "r");
  }
  while (1) {
    if (!bash) {
      myPrint("myshell> ");
      pinput = fgets(cmd_buff, 514, stdin);
    } else {
      pinput = fgets(cmd_buff, 514, bashFile);
      if (blankLine(pinput)) {
	continue;
      } else {
	myPrint(pinput);
      }
    }
    if (!pinput) {
      exit(0);
    }
    int len = strcspn(cmd_buff, "\n");
    if (len > 512) {
      myPrint(cmd_buff);
      printf("\n");
      write(STDOUT_FILENO, error_message, strlen(error_message));
      continue;
    }
    char command[len];
    char args[len];
    args[0] = 0;
    command[0] = 0;
    char output[len];
    int i, j, arg, redir, adv;
    i = j = arg = redir = adv = 0;
    while (i<len) {
      if (cmd_buff[i] == '+') {
	if (cmd_buff[i-1] == '>') {
	  adv = 1;
	  i++;
	  continue;
	}
      }
      if (cmd_buff[i] == ';') {
	if (arg) {
	  args[j] = 0;
	} else if (redir) {
	  output[j] = 0;
	} else {
	  command[j] = 0;
	}
	parseCommand(command, args, redir, adv, output);
	j = arg = redir = adv = 0;
	command[0] = 0;
	args[0] = 0;
	i++;
	continue;
      }
      if (cmd_buff[i] == '>') {
	if (arg) {
	  args[j] = 0;
	} else {
	  command[j] = 0;
	}
	redir = 1;
	arg = j = 0;
	i++;
	continue;
      }
      if (cmd_buff[i] == ' ' || cmd_buff[i] == '\t') {
	if (command[0] && !arg && !redir) {
	  command[j] = 0;
	  arg = 1;
	  j = 0;
	  i++;
	  continue;
	}
      }
      if (arg) {
	args[j] = cmd_buff[i];
      } else if (redir) {
	output[j] = cmd_buff[i];
      } else {
	command[j] = cmd_buff[i];
      }
      i++;
      j++;
    }
    if (arg) {
      args[j] = 0;
    } else if (redir) {
      output[j] = 0;
    } else {
      command[j] = 0;
    }
    parseCommand(command, args, redir, adv, output);
  }
  return 0;   
}
