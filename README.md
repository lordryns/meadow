# meadow
Tiny window manager for X11


It is safe to open the window manager directly from the tty but please note that it is riddled with bugs and at this stage you can not open any applications.

### Actual features 
- opens (yayy)
- does not crash (hopefully)
- basic pointer support 
- configurable (editable from `config.h`)
- Window rendering (still buggy)
- Application launcher (not really a feature, just uses `dmenu`)

### Things to be added 
- Internal shells
- Unique window display system


## How to run 

There are two ways to run this window manager

1. you can run the window manager as nested window using [Xephyr](https://en.wikipedia.org/wiki/Xephyr). To run simply install it and then using the provided `Makefile`, do:

```bash 
make build && make run
```

> [!NOTE]
> Always use `make run` as starting as `./meadow` will fail 


2. Run directly from the tty, this should be obvious but set your `.xinitrc` to `exec /path/to/meadow` before exiting your current window manager and launching meadow by running `startx`

## Default key bindings 

- Super + Q = Exit window manager
- Super + Space = Open application launcher (dmenu by default but can be changed in `config.h`)


Have fun :)
