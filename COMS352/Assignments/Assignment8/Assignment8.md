Ben Holmes
COMS 352
Assignment 8




11.9 
Consider a file system in which a file can be deleted and its disk space reclaimed while links
to that file still exist.What problemsmay occur if a newfile is created in the same storage area
orwith the same absolute path name? How can these problems be avoided?

ANSWER: 
If a user wants to open a file, the first file found on the VTOC of the disk will be opened. This could be a problem if the file was deleted.
To remedy this, just delete all links to a file after deleting the file. 




11.12 
Provide examples of applications that typically access files according to the following
methods:
• Sequential
• Random

ANSWER:
Sequential: Word processors, music players. 
Random: Realm, databases, video. 




11.17 
Some systems provide file sharing by maintaining a single copy of a file. Other systems
maintain several copies, one for each of the users sharing the file. Discuss the relative merits of
each approach.

ANSWER:
The more copies, the harder it is to keep all the files in sync with eachother. On the other hand, 
if there is only one file, it is easier to maintain the changes, and also requires less space. 




12.13 
Some file systems allow disk storage to be allocated at different levels of granularity. For
instance, a file system could allocate 4 KB of disk space as a single 4-KB block or as eight 512-byte
blocks. How could we take advantage of this flexibility to improve performance? What
modifications would have to be made to the free-space management scheme in order to support
this feature?

ANSWER:
We could take advantage of this flexibility by keeping track of the free blocks, and the current 
block being used. Then we would be able to allocate space dynmically by referring to these 
pointers. This type of allocation can reduce internal fragmentation. 



12.16 
Consider a file system that uses inodes to represent files. Disk blocks are 8 KB in size, and
a pointer to a disk block requires 4 bytes. This file system has 12 direct disk blocks, as well as
single, double, and triple indirect disk blocks. What is the maximum size of a file that can be
stored in this file system?

ANSWER:
(12 8 KB) + (2048 8 KB) + (2048 2048 8 KB) + (2048 2048 2048 * 8 KB) = 64 terabytes



12.17 
Fragmentation on a storage device can be eliminated by recompaction of the information.
Typical disk devices do not have relocation or base registers (such as those used when memory
is to be compacted), so how can we relocate files? Give three reasons why recompacting and
relocation of files are often avoided.

ANSWER:
How? 
First blocks have to be put into main memory and then transferred out to the secondary source. 

1.)Many disk files are not sequential. 
2.)Relocation requires a lot of register usage. 
3.)Relocation registers only apply to sequential files. 



7 
Run the following program and observe the output. The objective is to understand how the
open files are shared by the child and the parent process.

int main( )
{
 int fp;
 char buff[20];
 int pid;

 pid = fork( );
 if (pid==0)
 {
	 fp = open("sample", O_RDONLY);
	 printf("child begins %d\n" , getpid( ));
	 read(fp,buff,10);
	 buff[10] = '\0';
	 printf("child read: ");
	 puts(buff);
	 printf("\nchild exits");
 }
 else
 {
	 fp = open("sample", O_RDONLY);
	 printf("parent begins %d\n" , getpid( ));
	 read(fp,buff,10);
	 buff[10] = '\0';
	 printf("parent read: ");
	 puts(buff);
	 printf("parent exits\n");
  }
}


//OUTPUT
parent begins 24535
parent read: 
parent exits
child begins 24536
child read: 





