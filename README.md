### CSC 357 Assignment 3 - Memory Management

Purpose:
- This assignment focuses on implementing dynamic memory management in C through the creation of a dictionary (hash table) and a program that finds the most frequent words in a text file.
- The goal is to practice managing heap memory efficiently without memory leaks.
  
Ground Rules:
- Your code must compile without memory leaks using the gcc and valgrind commands.
- Programs must compile and run on the Cal Poly UNIX servers.
- Valgrind is used to check for memory leaks with full details on any leaks.
  
Dictionaries (Hash Table Implementation):
- Implement a dictionary using a hash table in dict.c. The dictionary maps string keys to values of arbitrary types (void pointers).
- Hash collisions are resolved using separate chaining.
- The backing array of the hash table should be resized when the load factor exceeds 1, rehashing the array from length n to 2n + 1.
- Functions must conform to the provided prototypes in dict.h. Helper functions are allowed but should not alter the provided function prototypes or structures.
- The dictionary is tested with unit tests from dicttests.c.

Word Frequencies Program:
- Implement a program called freq to find the most frequent words in a text file, with the following constraints:
- Words are delimited by whitespace.
- Words should be stripped of non-alphabetical characters, and all characters should be lowercased.
- Words with no alphabetical characters should be skipped.
- Words are sorted by frequency, with ties broken alphabetically.
- The program prints up to the top 10 most frequent words along with their frequencies.
- If an incorrect number of arguments is provided or if the file cannot be opened, the program should print an error message and exit with the appropriate status code.

Handling Input and Errors:
- If the user provides too few or too many arguments, the program must output an error message and terminate with exit status 1.
- If the input file cannot be opened, the program must print an error message and terminate with exit status 1.
- You are encouraged to use functions like fopen, fgets, strtok, strcmp, and qsort to read input, tokenize strings, and sort words.

Memory Management:
- Because the number of words and lines in the file may vary, the program should dynamically allocate memory for each word and line. Memory should be freed properly to avoid memory leaks.

Testing:
- The program is tested by comparing its output with the expected output using diff.
- The program must also pass Valgrind testing to ensure no memory leaks occur.
