# second Parent Selection

## Overview

This a simple implementation of the parent selection strategy in Leapfrog collaboration.

## Requirements

Leapfrog Parent Selection requires a system running:
* TSCH
* RPL
* the *RPL_DAG_MC_NSA_PNS* extension for putting the parent node set (PNS) of a node in the DIO/MC/NSA field
* the *PRE* extension for implementing Packet Replication and Elimination

## Getting Started

To use Leapfrog Parent Selection, add a global definitions, e.g in your `project-conf.h` file.

Set up the following callback:

```C
#define PRE_CONF_SELECT_REPLICATION_TARGETS_LOCAL_ADDRESSES leapfrog_parent_selection_select_replication_targets_local_addresses
```

To use Leapfrog Parent Selection, fist add it to your makefile `APPS` with `APPS += leapfrog-parent-selection`.

Make sure also that you have enabled the *RPL_DAG_MC_NSA_PNS* extension with a global definition, e.g.:
```C
#define RPL_CONF_WITH_MC 1
#define RPL_CONF_DAG_MC 1 /* RPL_DAG_MC_NSA */
#define RPL_CONF_DAG_MC_NSA_PNS 1

```

It is also suggested to define:
```
#define RPL_CONF_DAG_MC_NSA_MAX_ADDRESSES 2 /* at least 2 to be able to have a better chance of a valid alternative parent */
```

in order to allow more parents in the parent set.

Finally, make sure that you have enabled the *PRE* extension with a global definition, e.g.:
```C
#define WITH_PACKET_REPLICATION_AND_ELIMINATION 1
```

## Configuration

None.