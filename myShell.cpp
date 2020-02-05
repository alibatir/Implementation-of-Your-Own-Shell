/*
  Name: ALi BATIR
Number: 2015400261
*/
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<iostream> //Used to perform input and output operations
#include<string> //Perform string manipulation operations
#include<stdlib.h> //exit()
#include<sys/wait.h> //wait()
#include<bits/stdc++.h> //// using stringstream
#include <fcntl.h> //file control options
using namespace std;
//This function take values and create a child process in order to execute shell commands
void myfork(string command,string parameter){ //one command and one parameter 
	pid_t pid = fork(); // creating a child process 
	if (pid < 0){  
		fprintf(stderr, "Fork Failed"); 
	}else if (pid == 0){ // execution of the child process 
	
		//control the command
		if(command=="listdir" && parameter==""){ //only one command without parameter
       			 execlp("/bin/ls","ls",NULL);   //execute ls command

		}else if(command=="currentpath" && parameter==""){ //only one command without parameter
			execlp("/bin/pwd","pwd",NULL); //execute pwd command

	
		}else if(command=="listdir" && parameter=="-a"){ //listdir -a
			execlp ( "/bin/ls", "ls", "-a", NULL ); //execute ls -a command

		}else if(command=="printfile"){  //printfile myText.c
			const char *param=parameter.c_str();	//convert parameter(string) to const char*
			execlp("/bin/cat","cat",param,NULL); //execute cat fileName command
	
		}
		exit(0);
	}else { // execution of the parent process   	
		wait(NULL); // parent will wait for the child to complete 
	}
}

//This function takes values and creates a child process in order to execute shell commands with parameters
//in this function, the pipe() and  dup2() system calls are used in order to connect two seperate processes.
void myfork(string command,string parameter1,string parameter2,string parameter3,string parameter4){ //the count of input is five 

	const char *param1=parameter1.c_str(); //convert parameter(string) to const char*
	const char *param3=parameter3.c_str(); //convert parameter(string) to const char*

	//control commands and parameters	
	if(command=="printfile" && parameter2==">" && parameter4==""){	 //printfile text1.txt > text2.txt
		pid_t pid = fork(); // creating a child process 
		if (pid < 0){   //error occurred while creating a child process
			fprintf(stderr, "Fork Failed"); 
		}else if (pid == 0) { // execution of the child process 
			int read_fd=open(param1,O_RDONLY); //opened the file in read only mode (O_RDONLY) using the open() function and stored the file descriptor in the variable read_fd
			if (dup2(read_fd, 0) == 0) { //using dup2 system call to redirect file 
      				close(read_fd);  //close first file
   			 }else{
				exit(0);}
				
     			int write_fd =open(param3,O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR); //write second file and call flags in order to give permissions
			if (dup2(write_fd, 1) == 1) { //using dup2 system call to redirect file 
     				close(write_fd); //close second file
    			}else{
				exit(0);}
		
	     		execlp("/bin/cat", "cat",NULL); //cat for printfile 
			exit(0);
		}else{ // execution of the parent process
			wait(NULL); // parent will wait for the child to complete 
		}
	//control commands and parameters
	}else if(command=="listdir" && parameter1=="|" && parameter2=="grep" && parameter3!="" && parameter4==""){
		//pipe between to processes
		int pipe_fds[2]; //creating a array for pipe  
		pipe(pipe_fds); //creating a pipe by using pipe() system call
		int read_fd=pipe_fds[0]; //creating read file descriptor 
		int write_fd=pipe_fds[1]; //creating write file descriptor
		if (fork()==0) { // execution of the first child process 
 			 dup2(write_fd, 1);//copied the file descriptor into the standard output file descriptor STDOUT_FILENO (1) using dup2() function
 			 close(read_fd); //close read file descriptor
 			 close(write_fd);//close write file descriptor
 			 execlp("ls", "ls",NULL); //execute ls command
		}
		if (fork()==0) { // execution of the second child process 
 			 dup2(read_fd, 0); //copied the file descriptor into the standard input file descriptor STDIN_FILENO (0) using dup2() function
 			 close(read_fd);//close read file descriptor
 			 close(write_fd);//close write file descriptor
 			 execlp("grep", "grep",param3, NULL); //execute grep "argument" command
		}        // execution of the parent process 
		close(read_fd); //close read file descriptor
		close(write_fd); //close write file descriptor
		wait(NULL); // parent will wait for the first child to complete 
		wait(NULL); // parent will wait for the second child to complete 
					
	//control commands and parameters
	}else if(command=="listdir" && parameter1=="-a" && parameter2=="|" && parameter3=="grep" && parameter4!=""){
		const char *param4=parameter4.c_str(); //convert parameter(string) to const char*
		//pipe between to processes
		int pipe_fds[2]; //creating a array for pipe
		pipe(pipe_fds); //creating a pipe by using pipe() system call
		int read_fd=pipe_fds[0]; //creating read file descriptor 
		int write_fd=pipe_fds[1]; //creating write file descriptor
		if (fork()==0) { // execution of the first child process 
 			 dup2(write_fd, 1);//copied the file descriptor into the standard output file descriptor STDOUT_FILENO (1) using dup2() function
 			 close(read_fd); //close read file descriptor
 			 close(write_fd);//close write file descriptor
			 execlp("ls", "ls","-a",NULL); //execute ls command
		}
		if (fork()==0) { // execution of the second child process 
 			 dup2(read_fd, 0); //copied the file descriptor into the standard input file descriptor STDIN_FILENO (0) using dup2() function
 			 close(read_fd);//close read file descriptor
 			 close(write_fd);//close write file descriptor
 			 execlp("grep", "grep",param4, NULL); //execute grep "argument" command
		}        // execution of the parent process 
		close(read_fd); //close read file descriptor
		close(write_fd); //close write file descriptor
		wait(NULL); // parent will wait for the first child to complete 
		wait(NULL); // parent will wait for the second child to complete
	}
}
int main()

{
	string history[15]; //creating an arrar to keep history 	
	for(int i=0;i<=14;i++){ //giving array elements a value
	history[i]=""; //all of them are empty
	}
	string line=""; //creating a string variable for input 
	char* username = getenv("USER"); //using getenv() function to get username 
	while(line != "exit"){ //executing as soon as it is requested
		printf("%s >>> ",username); //shell process will be terminated after the user enters the exit command
		getline(cin,line);   //get the value which user enters

		if(line!=""){ //if input is no equal to empty value then add the input to history array
			for(int i=0;i<=14;i++){ //look at all element of array 
				if(history[i]==""){  //find first empty element in history array
				 history[i]=line; //add the value to first empty element
				 break;	//terminates  the execution of for loop	
				}
			}
	 		if(history[14]!=""){// if all elements in array are full of value,then swapping the elements  
				for(int i=0;i<=13;i++){	//the value of first element will be deleted	
				history[i]=history[i+1]; //the value of last element wiil be empty.
				} 
				history[14]=line; //giving the last element the value 
			}
		}
        	//find command and parameters in input 
		//splitting words in input
   	 	stringstream s(line); //using this for seperating words
		 string word;
   		 string words[5]; //creating array to get words 
   		 int wordscount = 0; // number of words 
   		 while (s >> word){ 
				words[wordscount]=word;  //adding words to the array        	
				wordscount++; //keep words count
 		}
		//determining command by entering number of words
		if(wordscount==1){       //only one command
			myfork(words[0],"");  //sending the command for fork and exec
	
		}else if(wordscount==2){   //one command and one parameter
			myfork(words[0],words[1]); //sending the values for fork and exec

		}else if(wordscount==4){ 	  
			if(words[0]=="printfile"  && words[2]==">"){ //printfile (fileName) > (newFileName)
				myfork(words[0],words[1],words[2],words[3],""); //sending values for fork and exec

			}else if(words[0]=="listdir" && words[1]=="|" && words[2]=="grep"){ //listdir | grep "argument"  
				 int w3_length=words[3].length(); //get the length of argument in order to delete quotes
			 	string words3=words[3].substr(1,w3_length-2); // get the argument in quotes
	     			 myfork(words[0],words[1],words[2],words3,""); //sending values for fork and exec 
			}
		}else if(wordscount==5){ //listdir -a | grep "argument"
			if(words[0]=="listdir" && words[1]=="-a" && words[2]=="|" && words[3]=="grep"){	
				 int w4_length=words[4].length(); //get the length of argument in order to delete quotes
				 string words4=words[4].substr(1,w4_length-2);// get the argument in quotes
			 	myfork(words[0],words[1],words[2],words[3],words4); //sending values for fork and exec  
			} 

		}	
		//print up to maximum 15 commands that are executed before
		if(line=="footprint"){  
			for(int i=0;i<=14;i++){ //get all element of array 
				if(history[i]!=""){
					printf("%d ",i+1);	//print the count of element in array
					cout<<history[i]<<endl; //print the values in the array
				}		
			}		
		} 
	}
	
            return 0;

}
