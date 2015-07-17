# v8-sandbox

Trying to get a better understanding how to embed `v8` in another library by following along tutorials such as:

- [google developers embed guide](https://developers.google.com/v8/embed)


## Usage

After cloning the repo you should be able to try each example via a simple `make run`.

Example:

```
cd 001-getting-started && make run
```

The very first time `v8` will get built. You can perform that step only by running `make` from any of the example
directories.

**Note**:
- you should have `g++` (for older v8 versions), `clang++`, `svn` (for older v8 versions) and `git` installed on your system.
- newer versions require `ninja` (`brew install ninja`) to speed up the build

## v8 versions

The examples and Makefile have been adapted to work with  v8 versions (stored at different at the following
branches/tags):

- [v8-3.14.5 used with node-0.10.26](https://github.com/thlorenz/v8-sandbox/tree/v8-3.14.5_node-0.10.26)
- [v8-3.21.17](https://github.com/thlorenz/v8-sandbox/tree/v8-3.21.17)
- [v8-3.24.22 used with node-0.11.12](https://github.com/thlorenz/v8-sandbox/tree/v8-3.24.22_node-0.11.12)
- [v8-3.25.30 used with node at commit c20b209dbb](https://github.com/thlorenz/v8-sandbox/tree/v8-3.25.30_node-c20b209dbb)
