#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int err(char *s, char *ss)
{
    while (*s)
        write(2, s++, 1);
    if (ss[0] != 0)
    {
         while (*ss)
            write(2, ss++, 1);
    }
    return (write(2,"\n",1));
}

int cd(char **av, int i)
{
    if (i != 2)
        return err("error: cd: bad arguments","");
    if (chdir(av[1]) == -1)
        return err("error: cd: cannot change directory to ", av[1]);
    return (0);
}

void set_pip(int is, int *fd, int e)
{
    if (is && (dup2(fd[e], e) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
        exit(err("error: fatal",""));
}

int exe(char **av, int i, char **env)
{
    int sts, fd[2], pid;
    int is_pip = av[i] && !strcmp(av[i],"|");

    if (!is_pip && !strcmp(*av,"cd"))
        return (cd(av, i));
    if (is_pip && pipe(fd) == -1)
        exit(err("error: fatal",""));
    if ((pid = fork()) == -1)
        exit(err("error: fatal",""));
    if (!pid)
    {
        av[i] = 0;
        if (!strcmp(*av,"cd"))
            exit(cd(av, i));
        set_pip(is_pip, fd, 1);
        execve(*av, av, env);
        exit(err("error: cannot execute ",*av));
    }
    waitpid(pid, &sts, 0);
    set_pip(is_pip, fd, 0);
    return (WIFEXITED(sts) && WEXITSTATUS(sts));
}

int main(int c,char **av, char **env)
{
    (void)c;
    int i = 0;
    int sts = 0;
    while(av[i])
    {
        av += i + 1;
        i = 0;

        while(av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
            i++;
        if (i)
            sts = exe(av, i ,env);
    }
    return (sts);
}