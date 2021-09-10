#ifndef __PRE_CONF_H__
#define __PRE_CONF_H__

/* How many unique packets to keep in history, for the purpose
 * of eliminating duplicated packets */
#ifdef PRE_CONF_UNIQUE_PACKET_HISTORY_SIZE
#define PRE_UNIQUE_PACKET_HISTORY_SIZE  PRE_CONF_UNIQUE_PACKET_HISTORY_SIZE
#else /* PRE_CONF_UNIQUE_PACKET_HISTORY_SIZE */
#define PRE_UNIQUE_PACKET_HISTORY_SIZE            8
#endif /* PRE_CONF_UNIQUE_PACKET_HISTORY_SIZE */

/* The maximum total number of nodes to which a replica will be sent to
 * If no replication is desired, then use 1 as the value.
 */
#ifdef PRE_CONF_MAX_REPLICATION_NODES
#define PRE_MAX_REPLICATION_NODES PRE_CONF_MAX_REPLICATION_NODES
#else
#define PRE_CONF_MAX_REPLICATION_NODES 1
#endif /* PRE_CONF_MAX_REPLICATION_NODES */

/* The function to call when PRE needs to get list of replication targets.
 * The default implementation used does nothing, just returns the default target address.
 */
#ifdef PRE_CONF_SELECT_REPLICATION_TARGETS_LOCAL_ADDRESSES
#define PRE_SELECT_REPLICATION_TARGETS_LOCAL_ADDRESSES PRE_CONF_SELECT_REPLICATION_TARGETS_LOCAL_ADDRESSES
#else
#define PRE_SELECT_REPLICATION_TARGETS_LOCAL_ADDRESSES pre_hooks_default_select_replication_targets_local_addresses
#endif /* PRE_CONF_SELECT_REPLICATION_TARGETS_LOCAL_ADDRESSES */

#endif /* __PRE_CONF_H__ */
