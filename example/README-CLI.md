# Command Line Interface (CLI) for yloc

## Introduction

The code in this directory can be compiled to get the `yloc-cli` executable.

The `yloc-cli` executable aims to provide quick access to the features of the dynamic topology information
system yloc.

It was designed as a tool to help software developers optimise their code for performance.

## Documentation

The following use cases are supported:

### Get overview of hardware topology

If used without any additional CLI option, the command will print a dot graph representation of the host machine to stdout.
```
yloc-cli
```

The output can also be written to a file.
```
yloc-cli -o graph.dot
```
The file extension of the output file is optional.
By default, the format of the output will be dot.
If the output file has a file extension that corresponds to one of the supported file formats, the format of the output will be adjusted automatically.

The output format can be specified without the need to use the corresponding file extension for an output file.
```
yloc-cli -f csv
```

If the options `-o` and `-f` are used together, the format given as argument for option `-f` will override the format which may have been derived from the file extension.
```
yloc-cli -f csv -o graph.dot
```

The supported output formats can be checked with the `-F` option.
```
yloc-cli -F
```

### Filter specific information

#### Filter by component type
Output can be filtered by specific component types.
Multiple types can be specified as comma-separated string.
It is possible to use either the full type name of a component or a substring to match multiple components (e.g. Core to match CPUCore & GPUCore).
```
yloc-cli -c 'Core,L1DataCache'
```

The supported component types that are present on the machine can be checked with the option `-C`.
```
yloc-cli -C
```
#### Filter by hardware property
Output can be filtered by specific hardware properties.
Multiple properties can be specified as comma-separated string.
The full property name must be used for each property that is to be included in the output.
Without the option `-p`, the output will contain every hardware property by default.
```
yloc-cli -p 'memory,numa_affinity'
```

The hardware properties supported by yloc and its active modules can be checked with the `-P` option.
```
yloc-cli -P
```


### Monitor hardware properties

To track the dynamic development of hardware properties, a time period can be specified with option `-d`.
The hardware properties will be probed in the default interval of 1000ms until the given probing period is reached.
The argument for option `-d` will be interpreted as period in milliseconds.
```
yloc-cli -d 8000
```
If an output-file is given and format is dot, the output-file will be overwritten for each sampling point (to enable live observation of a graph), while csv output will be appended.

By specifying a probing period of -1, an endless can be triggered.
```
yloc-cli -d '-1'
```

A custom probing interval can be specified with option `-i`.
The argument for option `-i` is interpreted as time interval between samples in milliseconds.
```
yloc-cli -i 4000
```

### Check the activated modules

The functionality of yloc can be extended by modules.
The option `-M` can be used to check which yloc modules are available for the host machine.
```
yloc-cli -M
```