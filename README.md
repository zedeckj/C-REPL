A simple REPL-like enviroment I threw together for compiling C JIT. User input is compiled and ran after each line entry.

```
>> char * str = "Hello World!";                                                                                                                                                                             
>> int x = 10 * 10;                                                                                                                                                                                         
>> if (x == 100) printf("%s\n", str);                                                                                                                                                                       
Hello World!                                                                                                                                                                                                
>> 
```                                                                                                                                                                                                 

Lines in the form of `(<expression>);` are printed without having to call any IO functions.

```
>> int x;                                                                                                                                                                                                   
>> x = 1 + 2 + 3 * 5;                                                                                                                                                                                       
>> (x);                                                                                                                                                                                                     
18                                                                                                                                                                                                          
>> double y = 3.4;                                                                                                                                                                                          
>> (y);                                                                                                                                                                                                     
3.400000                                                                                                                                                                                                    
>> (y * x);                                                                                                                                                                                                 
61.200001                                                                                                                                                                                                   
>> char * str = "Works for strings too!";                                                                                                                                                                   
>> (str);                                                                                                                                                                                                   
Works for strings too!
>>
```

This expression form is still compiled normally, with side effects and mutation occuring as expected:

```
>> int x = 0;                                                                                                                                                                                               
>> (x = 1);                                                                                                                                                                                                 
1                                                                                                                                                                                                           
>> (x);                                                                                                                                                                                                     
1                                                                                                                                                                                                           
>>           
```

```
>> (printf("What does printf return anyways?\n"));                                                                                                                                                          
What does printf return anyways?                                                                                                                                                                            
33                                                                                                                                                                                                          
>>      
```

Compilation errors gracefully exit the REPL without anything exploding:

```
>> int x = 3;                                                                                                                                                                                               
>> int y = 2;                                                                                                                                                                                               
>> int x = 1;                                                                                                                                                                                               
<stdin>:7:5: error: redefinition of 'x'                                                                                                                                                                     
int x = 1;                                                                                                                                                                                                  
    ^                                                                                                                                                                                                       
<stdin>:5:5: note: previous definition is here                                                                                                                                                              
int x = 3;                                                                                                                                                                                                  
    ^                                                                                                                                                                                                       
1 error generated.   
```

```
>> char a = 'a'; // Now I know what a equals                                                                                                                                                                
>> (a + b);                                                                                                                                                                                                 
<stdin>:6:6: error: use of undeclared identifier 'b'                                                                                                                                                        
(a + b);                                                                                                                                                                                                    
     ^                                                                                                                                                                                                      
1 error generated.        
```

The REPL can be closed with `exit(int)`, a return statement, or `quit()`. 

```
>> char * str = 0; // or bad malloc
>> if (!str) quit();  

```

A plain keyboard interrupt works as well.


By default, everything is run in the `main` function. This leads to defining top level declarations causing an error.

```
>> int foo(int x) { return x * 2; }                                                                                                                                                                         
<stdin>:5:16: error: function definition is not allowed here                                                                                                                                                
int foo(int x) { return x * 2; }                                                                                                                                                                            
               ^                                                                                                                                                                                            
1 error generated.                                                                                                                                                                                  
```

To get around this, the preprocessor is extended with the directive `#top`

```
>> #top int add1(int x) { return x + 1; }
>> (add1(2));
3
>> 
```

Other preprocessor directives are put outside of main as well, so `#include` works as one would hope.

```
>> (LLONG_MAX);
<stdin>:6:2: error: use of undeclared identifier 'LLONG_MAX'
(LLONG_MAX);
 ^
1 error generated.
```

```
>> #include <limits.h>
>> (LLONG_MAX);  
9223372036854775807
>>
```
