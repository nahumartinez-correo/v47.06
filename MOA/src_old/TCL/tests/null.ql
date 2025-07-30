drop table t;
create table t keys (:Cds_count, :Cds_num2) allow delete allow keychange buffer size 128;
create index name on t (:Cds_table_name);
create unique index date on t (:Cds_date sparse, :Cds_table_name);
create index fix on t (:Cds_float_field);
insert into t (:Cds_count, :Cds_num2, :Cds_table_name)
values (1, 1, one)
(2,2, two)
(3,3, three);

select from t where :Cds_date = "";
! sh "tail ./test/out |grep win"
! tail ./test/out |grep record
select from t where :Cds_float_field = "";
! tail ./test/out |grep win
! tail ./test/out |grep record
select from t where :Cds_table_name = "";
! tail ./test/out |grep win
! tail ./test/out |grep record
create unique index owner on t (:Cds_count, :Cds_num2, :Cds_owner);
select from t where :Cds_owner = "";
create index owner on t (:Cds_count, :Cds_num2, :Cds_owner);
select from t where :Cds_owner = "";
! tail ./test/out |grep win
! tail ./test/out |grep record

insert into t (:Cds_count, :Cds_num2, :Cds_table_name, :Cds_owner)
values (4, 4, four, me)
(5,5,five, myself)
(6,6, six, I);

select from t where :Cds_owner = "";
! tail ./test/out |grep win
! tail ./test/out |grep record
select :Cds_count, :Cds_owner from t where :Cds_owner > "";
! tail ./test/out |grep win
! tail ./test/out |grep record

drop index owner from t;
create index owner on t (:Cds_owner, :Cds_count);
select from t where :Cds_owner = "";
! tail ./test/out |grep win
! tail ./test/out |grep record
select :Cds_count, :Cds_owner from t where :Cds_owner > "";
! tail ./test/out |grep win
! tail ./test/out |grep record
select :Cds_count, :Cds_date from t where :Cds_date is_missing;
! tail ./test/out |grep dinged
! tail ./test/out |grep win
! tail ./test/out |grep record
select :Cds_count, :Cds_date from t where :Cds_count is_missing 99;
! tail ./test/out |grep win
! tail -13 ./test/out |grep record
