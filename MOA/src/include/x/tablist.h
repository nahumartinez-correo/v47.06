#ifndef TABLIST_H
#define TABLIST_H
#ifndef TABLE_STRUCT
#define TABLE_STRUCT
#define Table void
#endif
global_proc Table *find_table(const char *table_name);
global_proc Table *find_table_in(const char *internal_table_name);
global_proc Table *find_table_n(int table_number);
global_proc int cds_record_count(Table *table);
global_proc addr cdsTableSizes(void);
global_proc bool table_lists_equal(void *list1, void *list2);
global_proc short table_lists_diff(void *list1, void *list2, long *records_dif, long *bytes_dif, byte *table_number);
global_proc long log_position(void *list_headr);
global_proc void print_table_sizes(void *list1);
global_proc void free_table_space(Table *table);
global_proc void free_all_tables(void);
global_proc void un_link(const char *func_name, const char *file_name);
global_proc bool cdsCopyTables(void *ccb, char *machine, bool quick);
global_proc void	verify_tables(void);
#endif
