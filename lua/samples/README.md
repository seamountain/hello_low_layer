# SimpleGameLoop
## Requirement
- SDL2-2.0.3
  - Use source code, build and place this path
  - https://www.libsdl.org/download-2.0.php

Read README-macosx.txt
```sh
./configure
make
sudo make install # this sudo is needed?
```

## Solutions
### "library not found for -lSDL2"
https://github.com/PistonDevelopers/rust-empty/issues/175

Adding the below at .zshrc helped me

```sh
export LIBRARY_PATH="$LIBRARY_PATH:/usr/local/lib"
```
