#include "icssh.h"
#include <readline/readline.h>
#include "helpers.h"



int main(int argc, char* argv[]) {
	int exec_result;
	int exit_status;
	pid_t pid;
	pid_t wait_result;
	char* line;
	int exitflag = 0;
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


	//extra credit customized shell prompt
	char * theprompt = "";
	#ifdef DEBUG
	// char promptarray[100];

	//getting the username
	char * usernamestr = getenv("USER");
	if(usernamestr == NULL)
	{
		usernamestr = "";
	}

	// getting the hostname
	char * hostnamestr = getenv("HOSTNAME");
    if(hostnamestr == NULL)
	{
		hostnamestr = "";
	}


	// getting the pwd
	char * currentwdstr = getenv("PWD");
     if(currentwdstr == NULL)
	{
		currentwdstr = "";
	}
	
	// #define SHELL_PROMPT "\x1b[1;31m<53shell>$\x1b[0m "
	theprompt = createcustshellprompt(usernamestr,hostnamestr,currentwdstr);
	#endif


    	// print the prompt & wait for the user to enter commands string
	while ((line = readline(theprompt)) != NULL) {
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

				// printLList(testlist,stdout);


				if(index != -1)
				{
					node_t * tarptr = testlist->head;

					// printf("test print the pid %d\n",*((tarptr->value)->pid));

					//move the index
					for(int i = 0; i < index; i++)
					{
						tarptr = tarptr->next;
					}
					
					// tarptr+=index;

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
		if (strcmp(job->procs->cmd, "exit") == 0 || exitflag == 1) {
			//kill all background jobs
			node_t * mvnodeptr = testlist->head;

			while(mvnodeptr != NULL)
			{
				bgentry_t * bgptr = mvnodeptr->value;
				//kill the child
				// printf("the childpid %d\n",bgptr->pid);
				//print the bgterm of the killed child
				// if(exitflag == 0)
				// {

				// }
				printf(BG_TERM,bgptr->pid,bgptr->job->line);


				//kill
				kill(bgptr->pid,SIGTERM);

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
			//call free prompt line
			#ifdef DEBUG
			free(theprompt);
			#endif
			
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

				

				//update shell prompt
				//extra credit customized shell prompt
				#ifdef DEBUG
				// printf("updating the pwd\n");


				// getting the pwd
				currentwdstr = cwdstr;
				// printf("the new str %s\n",cwdstr);
				
				//update the shell prompt
				//free memory first
				free(theprompt);
				theprompt = createcustshellprompt(usernamestr,hostnamestr,currentwdstr);
				#endif

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
		//the extracredit builtins
		if(strcmp(job->procs->cmd, "ascii53") == 0)
		{
			printascii53();//print out the ascii

			//terminate shell afterwards no
			free(line);
			free_job(job);
			// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
            continue;
		}
		//the extracredit builtin fg
		if(strcmp(job->procs->cmd, "fg") == 0)
		{


			// printf("in the fg buildin\n");

			if(testlist->head != NULL)
			{
				//check the argc count	
				if(job->procs->argc == 1)
				{
					//default fg
					// printf("the default fg\n");

					int rearind = findrearindx(testlist);

					//get the node
					node_t * ndelastptr = testlist->head;
						
					//move the index
					for(int i = 0; i < rearind; i++)
					{
						// printf("moving \n");
						ndelastptr = ndelastptr->next;
					}

					bgentry_t * bgentlstptr = ndelastptr->value;

					//set the pid
					pid = bgentlstptr->pid;

					// printf("the pid %d\n",pid);

					//wait for the proc in fg
					wait_result = waitpid(pid, &exit_status, 0);
					if (wait_result < 0) {
						printf(WAIT_ERR);
						exit(EXIT_FAILURE);
					}

					//after completion remove rear
					removeByIndex(testlist,rearind);
					

					//remove the last node at the end
					// removeRear(testlist);

				}
				else
				{
					//given the pid
					// printf("given the pid %d\n",atoi((job->procs->argv)[1]));

					// printf("the chec str %d\n",myCheckStrIsDigit((job->procs->argv)[1]));

					if(myCheckStrIsDigit((job->procs->argv)[1]) == 0)
					{
						// printf("waiit error in pid\n");
						//the str is not all digits
						fprintf(stderr,PID_ERR);

						//terminate shell afterwards no
						free(line);
						free_job(job);
						// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
						continue;
					}

					

					int retindx = findinLL(testlist,atoi((job->procs->argv)[1]));

					//check if index exists
					if(retindx < 0)
					{
						//the pid DNE
						//the str is not all digits
						fprintf(stderr,PID_ERR);

						//terminate shell afterwards no
						free(line);
						free_job(job);
						// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
						continue;

					}
					
					

					// printf("the ret indx %d\n",retindx);

					//get the node
					node_t * ndelastptr = testlist->head;
						
					//move the index
					for(int i = 0; i < retindx; i++)
					{
						// printf("moving \n");
						ndelastptr = ndelastptr->next;
					}

					bgentry_t * bgentlstptr = ndelastptr->value;

					//set the pid
					pid = atoi((job->procs->argv)[1]);
					
					// printf("the pid %d\n",pid);

					//wait for the proc in fg
					wait_result = waitpid(pid, &exit_status, 0);
					if (wait_result < 0) {
						printf(WAIT_ERR);
						exit(EXIT_FAILURE);
					}

					//after completion remove the index
					removeByIndex(testlist,retindx);

				}

			}
			else
			{
				//print error
				fprintf(stderr,PID_ERR);
			}
			

			//terminate shell afterwards no
			free(line);
			free_job(job);
			// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
            continue;
		}

		

		//tesing ddebug
		// printf("teesting the pipes\n");

		
		
	

		// example of good error handling! 
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}

		


		//test for the case num proc is 1

		if(job->nproc == 1)
		{
			
			// printf("The num of proc is 1\n");

			//for the case where nproc is 1


			if (pid == 0) {  //If zero, then it's the child process

				//get the first command in the job list
				proc_info* proc = job->procs;
				// printf("child got %s\n",proc->cmd);
				

				//opening the files
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
					// printf("warning the indne\n");
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
				
				// printf("exec %s %s\n",proc->cmd,(proc->argv)[0]);
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
					sigset_t mask,prevmask;
					//set empty set
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
		

		
			
		}
		else
		{

			//for the case when num proc is > 1
			//remember a child has already been created so need nproc-1 childs
			// printf("the >1 nproc childs\n");

			//init proc
			//get the first command in the job list init and it moves
			proc_info* proc = job->procs;

			//The manager child
			if(pid == 0)
			{
				//blocking
				sigset_t mask,prevmask;

				sigemptyset(&mask);
				//add sigterm
				sigaddset(&mask,SIGTERM);

				//block the signal
				sigprocmask(SIG_BLOCK,&mask,&prevmask);

				
				// printf("the manager child\n");
				pid_t mpid;

				//install signal handler for term
				//test set up sigchild handler
				if (signal(SIGTERM, sigterm_handler_pipe) == SIG_ERR) {
					perror("Failed to set sigterm handler");
					exit(EXIT_FAILURE);
				}
				
				

				//test creating pipes
				//caclc the num of pipes
				int calcnumpipes = (job->nproc - 1)*2;
				// printf("the calc num of pipes %d\n",calcnumpipes);

				// int pd2[2];
				// pipe(pd2);

				//init the pipes
				int thepipelist[calcnumpipes];
				// // init the pipes
				// for(int i = 0;i<calcnumpipes;i = i + 2)
				// {
				// 	//call pip to create a new pipe
				// 	if(pipe(thepipelist + i) != 0)
				// 	{
				// 		//error
				// 		perror("error creating the pipes!\n");
				// 		exit(EXIT_FAILURE);
				// 	}
				// }

				//need to free the memory
				// Terminating the shell
				
				//get the first command in the job list
				// proc_info* proc = job->procs;

				//printing test
				

				//close read end

				//close stdout
				// close(STDOUT_FILENO);

				//close read end
				// close(thepipelist[0]);

				//set up first pipe set as out
				// dup2(thepipelist[1],STDOUT_FILENO);

				//test close the read end
				// close(thepipelist[0]);
				// //test close write end
				// close(thepipelist[1]);

				// //close the pipes
				// close(thepipelist[0]);//close read
				// close(thepipelist[1]);//cloase write

				//test set up list to kill child
				pid_t pipechildlist[job->nproc];
				int pipchildlistct = 0;

				//code for manager
				int bottomcounter = 1;

				int initpipecounter = 0;

				for(int i = 0;i<(job->nproc);i++)
				{
					//initialize the pipes
					if(i != (job->nproc)-1)
					{
						if(pipe((thepipelist + initpipecounter)) != 0)
						{
							//error
							perror("error creating the pipes!\n");
							exit(EXIT_FAILURE);
						}
					}
					
					
					

					//spawn more child process
					mpid = fork();
					if(mpid < 0)
					{
						perror("Warning the child fail to spawn\n");
						exit(EXIT_FAILURE);
					}

					if(mpid == 0)
					{
						if(i == (job->nproc-1))
						{
							//in the second ... child
							//the last child

							// printf("in last chlid %d\n",bottomcounter);
							// printf("the command: !!!!!!!! %s\n",proc->cmd);

							//setting up the input portion
							//close the stdin
							close(STDIN_FILENO);

							//close the write end
							close(thepipelist[bottomcounter-2]);//cloase write


							//set the in pipe to 0
							dup2(thepipelist[bottomcounter-3],STDIN_FILENO);

							//setting up the output portion if redirection exists
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

								//terminate shell afterwards no
								free(line);
								free_job(job);
								// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
								continue;
							}

							//using dup to change stdout descriptor
							if(job->out_file != NULL)
							{
								//change the stdout to the filedescriptor
								dup2(fdoutfile,STDOUT_FILENO);
								close(fdoutfile);
							}



							//redirection for stderr
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
								if(fdoutfile != -1)
								{
									// printf("closing the outfile\n");
									close(fdoutfile);
								}



								//terminate shell afterwards no
								free(line);
								free_job(job);
								// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
								continue;
							}

							//dup if errfile exists
							if(proc->err_file != NULL)
							{
								//dup the opened infile over to stdin
								dup2(fderrfile,STDERR_FILENO);
								//test close the fderrfile
								close(fderrfile);

							}

							//execve

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
						}
						else if(i == 0)
						{
							//get the first command in the job list
							// proc_info* proc = job->procs;

							//printing test
							// printf("firstchild %d\n",bottomcounter);
							// printf("the command: !!!!!!!! %s\n",proc->cmd);


							
							//setting up the stdin of proc if infileexists
							//check if in file exists
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
								// printf("warning the indne\n");
								//error the file does not exist
								fprintf(stderr,RD_ERR);

								//terminate shell afterwards no
								free(line);
								free_job(job);
								// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
								continue;

							}

							//dup if infile exists
							if(job->in_file != NULL)
							{
								//dup the opened infile over to stdin
								dup2(fdinfile,STDIN_FILENO);
								//test close the fdinfile
								close(fdinfile);

							}

							//redirection for stderr
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



								//terminate shell afterwards no
								free(line);
								free_job(job);
								// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
								continue;
							}

							//dup if errfile exists
							if(proc->err_file != NULL)
							{
								//dup the opened infile over to stdin
								dup2(fderrfile,STDERR_FILENO);
								//test close the fderrfile
								close(fderrfile);

							}

							

							//close read end of pipe and setting up the stdout for piping to next proc

							//close stdout
							close(STDOUT_FILENO);

							//close read end
							close(thepipelist[0]);

							//set up first pipe set as stdout
							dup2(thepipelist[bottomcounter],STDOUT_FILENO);

							

							//test close the read end
							// close(thepipelist[0]);
							// //test close write end
							// close(thepipelist[1]);

							// //close the pipes
							// close(thepipelist[0]);//close read
							// close(thepipelist[1]);//cloase write

							//execve

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
						}
						else
						{
							//not the last child
							//in the second ... child

							// printf("in middle chlid %d\n",bottomcounter);
							// printf("the command: !!!!!!!! %s\n",proc->cmd);

							//close read and write
							close(STDIN_FILENO);
							close(STDOUT_FILENO);

							

							//close the write end
							close(thepipelist[bottomcounter-2]);

							//set the in pipe to 0
							dup2(thepipelist[bottomcounter-3],STDIN_FILENO);

							
							//redirection for stderr
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

								//terminate shell afterwards no
								free(line);
								free_job(job);
								// validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated
								continue;
							}

							//dup if errfile exists
							if(proc->err_file != NULL)
							{
								//dup the opened infile over to stdin
								dup2(fderrfile,STDERR_FILENO);
								//test close the fderrfile
								close(fderrfile);

							}
							

							//close the read end
							close(thepipelist[bottomcounter-1]);//close read

							//set the out pipe to 0
							dup2(thepipelist[bottomcounter],STDOUT_FILENO);

							//close the pipes
							// close(pd2[0]);//close read
							// close(pd2[1]);//cloase write

							//execve

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

						}

					}
					

					//printing pid
					// printf("a pid: %d\n",mpid);
					
					// //close ?
					// close(thepipelist[1]);
					// close(pd2[1]);
					// if(i == 0)
					// {
					// 	close(thepipelist[1]);
						
					// }
					// if(i == 1)
					// {
					// 	close(thepipelist[3]);
					// }
					
					//add the pit to bg entry in case it needs to be kill
					pipechildlist[pipchildlistct] = mpid;
					// //update
					pipchildlistct++;

					
					




					//closing the files
					//close test pipes write end
					if(i != 0)
					{
					// 	// wait(NULL);
						// printf("closing %d\n",(bottomcounter-2));
						close(thepipelist[bottomcounter-2]);
						//test close read??
						close(thepipelist[bottomcounter-3]);
					}

					//update counter
					bottomcounter += 2;

					//move the proc struct
					proc = proc->next_proc;

					//update pipeinit counter
					initpipecounter += 2;

					
				}//end of for loop
				//in the parent

				//close all pipes in parent
				// int cntdel = 0;
				// for(int i = 0;i<(job->nproc - 1);i++)
				// {
				// 	close(thepipelist[cntdel]);
				// 	cntdel += 2;
				// }

				

				// close(thepipelist[0]);
				// close(thepipelist[2]);
				


				// printf("the last pid: %d\n",mpid);
					

				
					

					
					
					
					
				//the parent
				// close(thepipelist[0]);//close read
				// close(thepipelist[1]);//cloase write

				// //the parent
				// //close the pipes
				// //the parent??
				// close(pd2[0]);
				// close(pd2[1]);
				// wait(0);
				

				// waitpid(0);

				//wait for all the proc
				// for(int i = 0;i < job->nproc;i++)
				// {
				// 	wait(NULL);
				// }

				//unblock the signal
				sigprocmask(SIG_SETMASK,&prevmask,NULL);

				// //check if term sig is recieved
				// if(Stermflag == true)
				// {
				// 	//kill all the childs in list
				// 	for(int i = 0;i<job->nproc;i++)
				// 	{
				// 		printf("the KILLING %d\n",)
				// 	}
				// }


				// wait(0);
				
				int thereswaitpid;

				while((thereswaitpid = waitpid(mpid, &exit_status, WNOHANG)) == 0)
				{
					
					// printf("the wait pid res %d\n",thereswaitpid);

					//check if the signal flag is raised
					if(Stermflag == true)
					{
						// printf("killing the process\n");
						// use for loop

						for(int i = 0;i<job->nproc;i++)
						{
							// printf("KILLING %d\n",pipechildlist[i]);
							kill(pipechildlist[i],SIGKILL);
						}

						//break out of the while
						break;

					}
					
				}
				

				// printf("the wait result %d\n",theres);

				//case where there is still infity in bg
				for(int i = 0;i<job->nproc;i++)
				{
					// printf("KILLING %d\n",pipechildlist[i]);
					kill(pipechildlist[i],SIGKILL);
				}

				


				// Terminating the shell at the end of manager
				free(line);
				free_job(job);
						validate_input(NULL);   // calling validate_input with NULL will free the memory it has allocated


				//test call deletelist
				// printf("freeing list\n");
				deleteList(testlist);
				free(testlist);
				//call free prompt line
				#ifdef DEBUG
				free(theprompt);
				#endif
				

				//set exit flag
				// exitflag = 1;
				return 0;

				


			}


			//in the parent process

			if(job->bg == true)
			{
				//background jobs
				// printf("the background is true %d\n",pid);
				
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
				//forground job
				//wait for the manager proc to complete forground only pid is of the manager
				wait_result = waitpid(pid, &exit_status, 0);


				free_job(job);  // if a foreground job, we no longer need the data

			}
			
			

			//test by closing all the pipes
			// int cntdel = 0;
			// if(pid != 0)
			// {
			// 	for(int i = 0;i<(calcnumpipes);i++)
			// 	{
			// 		close(thepipelist[i]);
			// 		// cntdel += 2;
			// 	}
			// }
			

			

			

			
		}


		//free the line after wards
		free(line);

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
