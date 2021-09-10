CONTIKI_PROJECT = node
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI_WITH_IPV6 = 1
MAKE_WITH_ORCHESTRA ?= 0 # force Orchestra from command line
MAKE_WITH_STATIC_SCHEDULER ?= 0 # force static-scheduler from command line
MAKE_WITH_SECURITY ?= 0 # force Security from command line

#APPS_START 
APPS += orchestra
APPS += static-scheduler
APPS += second-parent-selection
APPS += powertrace

#APPS_END

#MODULES_START
MODULES += core/net/mac/tsch core/net/pre core/net/sequence-number core/net/saved-ext-pp-ap core/net/saved-ext-pa 
#MODULES_END

#CFLAGS_START 

ifeq ($(MAKE_WITH_CASE_6x5),1)
CFLAGS += -DWITH_CASE_6x5=1
endif

ifeq ($(MAKE_WITH_STATIC_SCHEDULER),1)
CFLAGS += -DWITH_STATIC_SCHEDULER=1
endif

ifeq ($(MAKE_WITH_PACKET_REPLICATION_AND_ELIMINATION),1)
CFLAGS += -DWITH_PACKET_REPLICATION_AND_ELIMINATION=1
endif

ifeq ($(MAKE_WITH_SEQUENCE_NUMBER),1)
CFLAGS += -DWITH_SEQUENCE_NUMBER=1
endif

ifeq ($(MAKE_WITH_ALTERNATIVE_PARENT_SELECTION),1)
CFLAGS += -DWITH_ALTERNATIVE_PARENT_SELECTION=1
endif

#CFLAGS_END


include $(CONTIKI)/Makefile.include
