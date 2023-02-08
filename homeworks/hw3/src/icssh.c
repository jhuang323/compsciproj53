#include "icssh.h"
#include <readline/readline.h>
#include "helpers.h"
//mines
// #include "linkedList.h"



int main(int argc, char* argv[]) {
	int exec_result;
	int exit_status;
	pid_t pid;
	pid_t wait_result;
	char* line;
#ifdef GS
    rl_outstream = fopen("/dev/null", "w");
#endif

	//test set up sigchild handler
	if (signal(SIGCHLD, sigchild_handler) == SIG_ERR) {
		perror("Failed to set sigchild handler");
		exit(EXIT_FAILURE);
	}
	// signal(SIGCHLD, sigchild_handler);

	//test custom sigusr2
	// signal(SIGUSR2,siguser2_handler);
	if (signal(SIGUSR2,siguser2_handler) == SIG_ERR) {
		perror("Failed to set sigusr2 handler");
		exit(EXIT_FAILURE);
	}

	// Setup segmentation fault handler
	if (signal(SIGSEGV, sigsegv_handler) == SIG_ERR) {
		perror("Failed to set signal handler");
		exit(EXIT_FAILURE);
	}

	

	//create own list_t that stores bgentry_t
	List_t * testlist = CreateList(&BgentryTimeComparator); //need to delete after wards


    	// print the prompt & wait for the user to enter commands string
	while ((line = readline(SHELL_PROMPT)) != NULL) {
        	// MAGIC HAPPENS! Command string is parsed into a job struct
        	// Will print out error message if command string is invalid
		job_info* job = validate_input(line);
        	if (job == NULL) { // Command was empty string or invalid
			free(line);
			continue;
		}

        	//Prints out the job linked list struture for debugging
        	#ifdef DEBUG   // If DEBUG flag removed in makefile, this will not longer print
            		debug_print_job(job);
        	#endif

		//test flag for removing zombies
		if(Scflag == true)
		{
			// printf("sig child flag is true\n");

			pid_t testwaitpit;

			while((testwaitpit = waitpid((pid_t)-1,0,WNOHANG))> 0)
			{
				// printf("the pid returned: %d\n",testwaitpit);

				//remove it in the LL if it exists
				int index = findinLL(testlist,testwaitpit);
				// printf("the index: %d\n",index);

				if(index != -1)
				{
					node_t * tarptr = testlist->head;

					//add index
					tarptr+=index;

					bgentry_t * tarbgent = tarptr->value;
					//print out background
					printf(BG_TERM,tarbgent->pid,tarbgent->job->line);

					//delete the node
					removeByIndex(testlist,index);
				}
				

				//set flag to false
				Scflag = false;

			}

			

			//set global flag to false afterwards
		}
		// else
		// {
		// 	printf("sig child flag is false!\n");
		// } 

		//test redirection
		//test opening files
		// if(job->in_file != NULL)
		// {
		// 	printf("infile is not NULL\n");
		// 	int finfileptr = open(job->in_file,O_RDONLY);

		// 	//set file descriptor for in
		// 	dup2(finfileptr,STDIN_FILENO);

		// }



		//for the sigusr2flag
		// if(Susr2flag == true)
		// {
		// 	// printf("the pid is %d\n",getpid());
		// 	printf("Hi User! I am process %d\n",getpid());
		// 	//set back to false
		// 	Susr2flag = false;
		// }

		//test my function for error checking filein, fileout, filerr nodes
		int retvval = errorcheckfilesgivenvalid(job->in_file,job->out_file,job->procs);

		// printf("the retval: %d\n",retvval);

		//if the error check is notvalid print msg
		if(retvval == 0)
		{
			//print error
			printf(RD_ERR);

			//terminate shell afterwards no
			free(line);
			free_job(job);
			// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
            continue;
		}

		// example built-in: exit
		if (strcmp(job->procs->cmd, "exit") == 0) {
			//kill all background jobs
			node_t * mvnodeptr = testlist->head;

			while(mvnodeptr != NULL)
			{
				bgentry_t * bgptr = mvnodeptr->value;
				//kill the child
				// printf("the childpid %d\n",bgptr->pid);
				//print the bgterm of the killed child
				printf(BG_TERM,bgptr->pid,bgptr->job->line);


				//kill
				kill(bgptr->pid,SIGKILL);

				//update the ptr
				mvnodeptr = mvnodeptr->next;
			}

			// Terminating the shell
			free(line);
			free_job(job);
            		validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated


			//test call deletelist
			// printf("freeing list\n");
			deleteList(testlist);
			free(testlist);
            		return 0;
		}

		//test built in command cd
		if(strcmp(job->procs->cmd, "cd") == 0)
		{
			// printf("Changing directory\n");

			// chdir
			//check if argc > 1, ingore additional arguments
			int cdreti = 0;
			if(job->procs->argc > 1)
			{
				// for when the directory is specified
				char * thepathname = (job->procs->argv)[1];
				// printf("the gotten dir: %s\n",thepathname);
				cdreti = chdir(thepathname);
				// printf("the return vall: %d\n",cdreti);
			}
			else
			{

				// printf("changing to home\n");
				char * thepathname = getenv("HOME");
				// printf("%s\n",thepathname);

				cdreti = chdir(thepathname);

				// printf("the return vall def: %d\n",cdreti);

			}

			//check if error occured 
			if(cdreti == -1)
			{
				// printf("Error occured\n");

				//change unsuccessful
				fprintf(stderr,DIR_ERR);
			}
			else
			{
				//change was successful
				// printf("change success\n");

				char * cwdstr = getcwd(NULL,0);
				// printf("test getcwd %s\n",cwdstr);

				fprintf(stdout,"%s\n",cwdstr);

				//free str from getcwd
				free(cwdstr);
			}

			//terminate shell afterwards no
			free(line);
			free_job(job);
			// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
            continue;


		}

		//test estatus
		if(strcmp(job->procs->cmd, "estatus") == 0)
		{
			//print exit status
			printf("%d\n",WEXITSTATUS(exit_status));

			//terminate shell afterwards no
			free(line);
			free_job(job);
			// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
            continue;
		}

		//test bglist
		if(strcmp(job->procs->cmd, "bglist") == 0)
		{
			//call print ll
			printLList(testlist,stderr);

			//terminate shell afterwards no
			free(line);
			free_job(job);
			// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
            continue;

		}

		
	
		

		// example of good error handling! 
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {  //If zero, then it's the child process

            	//get the first command in the job list
		    	proc_info* proc = job->procs;

				//open the infile, outfile ???

		
				//test openfiles for infile
				int fdinfile = -1;
				if(job->in_file != NULL)
				{
					fdinfile = open(job->in_file,O_RDONLY);

					
				}

				// printf("the fdinfo %d\n",fdinfile);

				//check if the infile could not be open
				if(job->in_file != NULL && fdinfile < 0)
				{
					//error the file does not exist
					fprintf(stderr,RD_ERR);

					//terminate shell afterwards no
					free(line);
					free_job(job);
					// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
					continue;

				}
				

				//test openfile for outfile
				int fdoutfile = -1;
				if(job->out_file != NULL)
				{
					// printf("opening a outfile\n");
					fdoutfile = open(job->out_file,O_WRONLY|O_CREAT|O_TRUNC,0777);
					// printf("the fdoutfo %d\n",fdoutfile);
				}

				// printf("the fdoutfo %d\n",fdoutfile);

				//what happens when you cannot access the file ??
				if(job->out_file != NULL && fdoutfile < 0)
				{
					if(fdinfile != -1)
					{
						// printf("closing the infile\n");
						close(fdinfile);
					}

					//terminate shell afterwards no
					free(line);
					free_job(job);
					// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
					continue;
				}

				//init for fd for stderr
				int fderrfile = -1;

			//try to open the std err file
			if(proc->err_file != NULL)
			{
				fderrfile = open(proc->err_file,O_WRONLY|O_CREAT|O_TRUNC,0777);
			}

			//check the if the file cannot be accessed
			if(proc->err_file != NULL && fderrfile < 0)
			{
				if(fdinfile != -1)
				{
					// printf("closing the infile\n");
					close(fdinfile);
				}

				if(fdoutfile != -1)
				{
					// printf("closing the infile\n");
					close(fdoutfile);
				}



				//terminate shell afterwards no
				free(line);
				free_job(job);
				// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
				continue;
			}


			//dup for the stdin infile if exist
			if(job->in_file != NULL)
			{
				dup2(fdinfile,STDIN_FILENO);
			}
			//dup for stdout outfile if exists
			if(job->out_file != NULL)
			{
				dup2(fdoutfile,STDOUT_FILENO);
			}
			//dup for stderr errfile if exists
			if(proc->err_file != NULL)
			{
				dup2(fderrfile,STDERR_FILENO);
			}

			//after dup close the file
			//close a infile test
			if(fdinfile != -1)
			{
				// printf("closing the infile\n");
				close(fdinfile);
			}
			if(fdoutfile != -1)
			{
				// printf("closing the outfile\n");
				close(fdoutfile);
			}
			if(fderrfile != -1)
			{
				// printf("closing the errfile\n");
				close(fderrfile);
			}
			

			exec_result = execvp(proc->cmd, proc->argv);
			if (exec_result < 0) {  //Error checking
				printf(EXEC_ERR, proc->cmd);
				
				// Cleaning up to make Valgrind happy 
				// (not necessary because child will exit. Resources will be reaped by parent)
				free_job(job);  
				free(line);
    				validate_input(NULL);  // calling validate_input with NULL will free the memory it has allocated

				exit(EXIT_FAILURE);
			}
		} else {
            		// As the parent, wait for the foreground job to finish
					//check if fg process
			if(job->bg == true)
			{
				//background jobs
				// printf("the background is true\n");
				
				//add bgstruct to the List_t
				bgentry_t * newbgentry = malloc(sizeof(bgentry_t));

				newbgentry->job = job;
				newbgentry->pid = pid;
				newbgentry->seconds = time(NULL);
				insertInOrder(testlist,(void*) newbgentry);

				//test free index
				// removeByIndex(testlist,0);

				// printf("the background pid: %d\n",pid);
				// printf("time: %ld\n",newbgentry->seconds);

				// printLList(testlist,stdout);
			}
			else
			{
				//foreground job
				
				wait_result = waitpid(pid, &exit_status, 0);
				if (wait_result < 0) {
					printf(WAIT_ERR);
					exit(EXIT_FAILURE);
				}


				free_job(job);  // if a foreground job, we no longer need the data
			}
			
		}

		
		free(line);

		//free the infile ??
		//close a infile test
		// if(fdinfile != -1)
		// {
		// 	// printf("closing the infile\n");
		// 	close(fdinfile);
		// }
		// if(fdoutfile != -1)
		// {
		// 	// printf("closing the outfile\n");
		// 	close(fdoutfile);
		// }
		// if(fderrfile != -1)
		// {
		// 	// printf("closing the errfile\n");
		// 	close(fderrfile);
		// }
		
	}

    	// calling validate_input with NULL will free the memory it has allocated
    	validate_input(NULL);
		


#ifndef GS
	fclose(rl_outstream);
#endif
	return 0;
}
