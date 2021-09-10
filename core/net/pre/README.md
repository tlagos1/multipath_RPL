# Packet Replication and Elimination (PRE)

## Overview

PRE implements the "Packet Replication and Elimination" mechanism.
In order to uniquely identify packets it adds an IPv6 Hop-by-Hop extension header TLV.
RPL already uses the Hop-by-Hop extension header to add the RPL Option TLV.
PRE extends the functionality to also add a new TLV containing a unique sequence number.
This sequence number can be use together with other packet information (e.g., source and remote addresses and ports)
to uniquely identify a packet.

## Requirements

PRE requires a system running RPL.
RPL is not required for any other reason than that RPL takes over the creation of the Hop-by-Hop
extension header and as a result RPL functions are the place where the sequence number TLV needs
to also be added. This was done in order not to completely rewrite the RPL IPv6 extension handling.
Also, only one Hop-by-Hop extetension header is allowed, so the TVL had to be inserted into the RPL-created Hop-by-Hop extension header.

## Getting Started

To use PRE, enable PRE:
`#define WITH_PACKET_REPLICATION_AND_ELIMINATION 1`

or to be able to configure it from the command line , add in the `Makefile`:
```Makefile
ifeq ($(MAKE_WITH_PACKET_REPLICATION_AND_ELIMINATION),1)
CFLAGS += -DWITH_PACKET_REPLICATION_AND_ELIMINATION=1
endif
```
and then append `MAKE_WITH_PACKET_REPLICATION_AND_ELIMINATION=1` to the `make` command line parameters.

## Configuration

Set `PRE_CONF_SELECT_REPLICATION_TARGETS_LOCAL_ADDRESSES` to the name of a callback with the signature of 
`pre_hooks_default_select_replication_targets_local_addresses` in `pre-hooks.h` to change how the replication
targets are selected.

Set `PRE_CONF_MAX_REPLICATION_NODES` to the max number of replications to support.

Set `PRE_CONF_UNIQUE_PACKET_HISTORY_SIZE` to the number of unique packets to keep in history, for the purpose
of eliminating duplicated packets.