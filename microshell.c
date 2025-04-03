#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void err(char *str)
{
    while (*str)
        write(2, str++, 1);
}

int ft_cd(char **av, int i)
{
    if (i != 2)
        return (err("error: cd: bad arguments\n"), 1);
    if (chdir(av[1]) == -1)
        return (err("error: cd: cannot change directory to "), err(av[1]), err("\n"), 1);
    return 0;
}

void setpip(int has_pip, int *fd, int end)
{
    if (has_pip && (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
    {
        err("error: fatal\n"), exit(1);
    }
}

int exce(char **av, int i, char **env)
{
    int has_pip, status, fd[2], pid;
    has_pip = av[i] && !strcmp(av[i], "|");

    if (!has_pip && !strcmp(*av, "cd"))
        return (ft_cd(av, i));

    if (has_pip && pipe(fd) == -1)
        err("error: fatal\n"), exit(1);

    if ((pid = fork()) == -1)
        err("error: fatal\n"), exit(1);
    if (!pid)
    {
        av[i] = 0;
        setpip(has_pip, fd, 1);
        if (!strcmp(*av, "cd"))
            exit(ft_cd(av, i));
        execve(*av, av, env);
        err("error: cannot execute "), err(*av), err("\n"), exit(1);
    }
    waitpid(pid, &status, 0);
    setpip(has_pip, fd, 0);
    return (WIFEXITED(status) && WEXITSTATUS(status));
}
int main(int ac, char **av, char **env)
{
    (void)ac;
    int i = 0;
    int status = 0;

    while (av[i])
    {
        av += i + 1;
        i = 0;
        while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
            i++;
        if (i)
            status = exce(av, i, env);
    }
    return (status);
}