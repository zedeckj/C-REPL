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


