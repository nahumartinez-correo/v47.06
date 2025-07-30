#define LOG_DEFINITION_VERSION 1

enum log_actions {
	RL_INSERT,
	RL_REPLACE,
	RL_REPLACE_WITH_INDEX_CHANGE,
	RL_UPDATE_FLDS,
	RL_DELETE,
	RL_COMPRESS,
	RL_QUIET,
	RL_COMPCOPY,
	RL_CREATE_TABLE,
	RL_DROP_TABLE,
	RL_RENAME_TABLE,
	RL_ALTER_TABLE,
	RL_COPY_TABLE_DEF,
	RL_CREATE_INDEX,
	RL_DROP_INDEX,
	RL_CANCEL_WORK,
	RL_PRIMARY_DIED,
	RL_SHADOW_DIED,
	RL_PHASE1,
	RL_PHASE2,
	RL_NODE_SPLIT,
	RL_NODE_MERGE,
	RL_BATCH_INSERT,
	RL_NULL,	/* This must always be the last action code.  Add new ones above. */
	RL_OPEN=64,	/* Instruction to echo server */
	RL_CLOSE,	/* instruction to echo server */
	RL_RESET,	/* instruction to echo server */
	RL_BEGIN_FLUSHING,	/* instruction to echo server */
	RL_UNLOG,	/* instruction to echo server */
	RL_SEEK,	/* instruction to log server */
	RL_READ_BACK, /* instruction to log server */
	RL_READ_FORE, /* instruction to log server */
	RL_FIND_EOF, /* instruction to log server */
	RL_LAST_ACT, /* instruction to log server */
	RL_LAST_CLEAN, /* instruction to log server */
	RL_IS_OPEN,
	RL_SEEK_EOF_SET,	/* seek to pos and set eof there */
	RL_PRIME_PUMP /* instruction to echo server to do nothing */
};

