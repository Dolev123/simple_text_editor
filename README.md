# Simple Text Editor

This is a very simple text editor for linux, that was enspired by [Making Minimalist Text Editor in C on Linux](https://www.youtube.com/watch?v=gnvDPCXktWQ) by [Nir Lichtman](https://github.com/nir9).  

## Compile

currently, only native compilation is supported:
```sh
make
```

create a test file:
```sh
make test_file
```

clean directories:
```sh
make clean
```

## Usage

The main premise for the text editor, is making a small change in an existing file (mainly configuration in mind), and not to write chunks of text/or creating a new file.
Therefore no charecter curser exsit, only a current line, that can be operated upon.
  
Basic Operations:
- [__p__]rint [_size_] : print a snippet of `size` rows above and below.
- [__j__]ump _dst_ : jump to row `dst`, use `-/+` to jump `dst` rows up/down.
- [__h__]elp : print all available commands, and their short form.
