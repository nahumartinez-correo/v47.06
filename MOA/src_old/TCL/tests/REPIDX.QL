drop table t;
create table t keys(:Cds_count serial) buffer size 100 allow delete allow keychange allow newfield;
create index ts on t (:Cds_index_name, :Cds_table_name) allow keychange;
insert into t (:Cds_index_name, :Cds_table_name)
values(aa,aaa)
(aa,aab)
(aa,bbb3)
(aa,bba4)
(aa,bbc5)
(aa,bbd6)
(aa,bbe7)
(aa,bbf8)
(aa,bbg9)
;
begin work;
insert into t (:Cds_index_name, :Cds_table_name)
values(bb,aaa);
commit work;  forcing quiet point in recovery log
update t set :Cds_table_name = "test" where :Cds_count = 1;
update t set :Cds_table_name = "2test" where :Cds_count = 1;
update t set :Cds_count = 777 where :Cds_table_name = "2test";
update t set :Cds_error_message = "12345678901234567890123456789012345678901234567890"
where :Cds_count = 777;


