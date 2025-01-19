```mermaid
graph TD;
	"|" --> "cat";
	"cat" --> "<<";
	"<<" --> "EOF";
	"EOF" --> "|";
	"|" --> "ls";
	"ls" --> "$VAR1";
	"$VAR1" --> "/";
	"/" --> "|";
	"|" --> "echo";
	"echo" --> "$ENV2 | ls | cat";
	"$ENV2 | ls | cat" --> "|";
	"|" --> "cat";
```
