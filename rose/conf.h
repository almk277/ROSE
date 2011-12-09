#ifndef ROSE_CONF_H
#define ROSE_CONF_H

#define STACK_SIZE             2048

#define REF_CAT_BITS           10
#define REF_TBL_BITS           10

#define ADDRCAT_SIZE           (1 << REF_CAT_BITS)
#define ADDRTBL_SIZE           (1 << REF_TBL_BITS)

#define ENABLE_DEBUGGER

#endif

