# Makefile

## Cool Operators

- `$@`: The current target
- `$<`: The first dependency

```Makefile
my_file: file.c 
    $@ # my_file
    $< # file.c
```
