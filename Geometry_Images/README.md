## Env support

### Lang

- Python
- CPP

### OS

- macos
- linux(ubuntu 20.04LTS) [untested]
- windows [untested]

## Static

- `Geometry_Images.pdf` paper file
- `*.m` mesh file
- `images/` RGB file for remesh

## Usage

```bash
sh run.sh <lang> <meshfile>
```

### example

```bash
sh run.sh cpp static/dragon1K.m
```

```bash
sh run.sh python static/gargoyle500.m
```