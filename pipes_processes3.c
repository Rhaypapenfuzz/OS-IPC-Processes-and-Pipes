#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>


/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipe1fd[2];
	int pipe2fd[2];
  int pid;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
	
	pipe(pipe1fd);
	pipe(pipe2fd);
  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pid = fork();
	if (pid < 0) 
	{ 
			fprintf(stderr, "fork Failed" ); 
			return 1; 
	} 
  
 else if (pid == 0)
	{
			
			int qid = fork();	
			if (qid < 0) 
			{ 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
			} 
			else if (qid == 0)
    {
      // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe
      dup2(pipe2fd[0], 0);
			// output to stdout (already done)
			// close fds
			close(pipe2fd[1]);
			close(pipe1fd[1]);
			close(pipe1fd[0]);

      // execute sort
			char *sort_args[] = {"sort", NULL};
      execvp("sort", sort_args);
			close(pipe2fd[0]);
    } 
	  else 
		{
			// child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe
			
      dup2(pipe1fd[0], 0);
			dup2(pipe2fd[1], 1);

      // close unused hald of pipe

			close(pipe1fd[1]);
			close(pipe2fd[0]);
      // execute grep

      execvp("grep", grep_args);
		  close(pipe1fd[0]);
			close(pipe2fd[1]);
		}
	 }
	
  else
	{
		// parent gets here and handles "cat scores"

		// replace standard output with output part of pipe


		// close unused unput half of pipe
		dup2(pipe1fd[1], 1);
		close(pipe1fd[0]);
		close(pipe2fd[0]);
		close(pipe2fd[1]);
		// execute cat
		
		execvp("cat", cat_args);
	}
}