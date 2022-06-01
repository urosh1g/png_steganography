# Description
`pngsteg` is a program that 'hides' your text inside a png image. It does this by creating a new private chunk, and inserting it after the IHDR chunk.
You can still view and edit your png files, and have the hidden text inside it after!

# Building
You'll need GCC for compilation. Just run
```shell
$ ./build.sh
```
and you'll get your pngsteg executable.

# Usage
In order to insert some text inside your png:
```shell
$ ./pngsteg -f input.png -o output.png -t 'hidden text'
```
In order to read hidden text:
```shell
$ ./pngsteg -f input.png
```
![Usage example](https://media.giphy.com/media/tgAw12rjPO2AexwehN/giphy.gif)

# References
For more information about the png format and chunks visit:
[libpng site](http://www.libpng.org/pub/png/spec/1.2/PNG-Structure.html)
