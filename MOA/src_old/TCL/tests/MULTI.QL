connect to tst2;

create table tst2:servers index (common:Cds_server_name) allow delete
buffer size 32;
insert into servers (:Cds_server_name) values (tst3);
connect using servers;
select :Cds_table_name from tst1:tables;
select :Cds_table_name from tst2:tables;
select :Cds_table_name from tst3:tables;
select :Cds_table_name from tst4:tables;  This should fail

create table servers index (:Cds_table_name) allow delete buffer size 32;
;  above create should fail.
create tst1:t1tab1 index (:Cds_table_name) allow delete buffer size 128;
create table t1tab2 index (:Cds_table_name) allow delete buffer size 128;

create table tst2:t2tab1 index (:Cds_table_name) allow delete buffer size 128;
create table tst2:t2tab2 index (:Cds_table_name) allow delete buffer size 128;

create table tst3:t3tab1 index (:Cds_table_name) allow delete buffer size 128;
create table tst3:t3tab2 index (:Cds_table_name) allow delete buffer size 128;

create table duplicate index (:Cds_table_name) allow delete buffer size 128;
create table tst1:duplicate index (:Cds_table_name) allow delete buffer size 128;
create table tst2:duplicate index (:Cds_table_name) allow delete buffer size 128;
create table tst3:duplicate index (:Cds_table_name) allow delete buffer size 128;
create table duplicate index (:Cds_table_name) allow delete buffer size 128;
insert into duplicate (:Cds_table_name :Cds_table_no) values (default 1);
insert into tst1:duplicate (:Cds_table_name :Cds_table_no) values (tst1 1);
insert into tst2:duplicate (:Cds_table_name :Cds_table_no) values (tst2 1);
insert into tst3:duplicate (:Cds_table_name :Cds_table_no) values (tst3 1);

select :Cds_table_name from tables;
select :Cds_table_name from tst1:tables;
select :Cds_table_name from tst2:tables;
select :Cds_table_name from tst3:tables;
select from servers;
select from t1tab1;
select from t1tab2;
select from t2tab1;
select from t2tab2;
select from t3tab2;
select from duplicate;
select from tst1:duplicate;
select from tst2:duplicate;
select from tst3:duplicate;

drop table duplicate;
drop table duplicate;
drop table duplicate;
drop table duplicate;	should fail
drop table tst1:duplicate; 	should fail
drop table tst2:duplicate;	should fail
drop table tst3:duplicate;	should fail

q;
