# ImRt Documentation

With this directory a documentation of the ImRt framework can be generated from
the source code using the `doxygen` tool. For instructions on generating
offline docs, see below.

These HTML docs can also be [found online](https://xxx.xxx/xxx).

# Generating Offline HTML Documentation

## Dependencies

- doxygen
- graphviz (to generate inheritance diagrams)

## Building

In a terminal run
```bash
doxygen imrt-doxygen-config
```
from within the directory with the "imrt-doxygen-config" file .

This will create a new subdirectory "html" containing a "index.html" that you can open in your browser to access the generated documentation.
