
; select :Cds_table_name :Cds_table_no from at least 30 different tables


select :Cds_table_name :Cds_table_no from test1;

select :Cds_table_name :Cds_table_no from test100;

select :Cds_table_name :Cds_table_no from test200;

select :Cds_table_name :Cds_table_no from test258;

select :Cds_table_name :Cds_table_no from test2;

select :Cds_table_name :Cds_table_no from test54 where :Cds_table_name == "test54";

select :Cds_table_name :Cds_table_no from test88;

select :Cds_table_name :Cds_table_no from test99;

select :Cds_table_name :Cds_table_no from test254 where :Cds_table_name == "test254";

select :Cds_table_name :Cds_table_no from test253;

select :Cds_table_name :Cds_table_no from test101;

select :Cds_table_name :Cds_table_no from test202;

select :Cds_table_name :Cds_table_no from test253;

drop table test254;

select :Cds_table_name :Cds_table_no from test254;

select :Cds_table_name :Cds_table_no from test253;

select :Cds_table_name :Cds_table_no from test10;

select :Cds_table_name :Cds_table_no from test20;

update test111 set :Cds_table_no = 2;

select :Cds_table_name :Cds_table_no from test111;

update test111 set :Cds_table_no = 1;

select :Cds_table_name :Cds_table_no from test111;

delete from test111 where :Cds_table_no == "1";

select :Cds_table_name :Cds_table_no from test111;

select :Cds_table_name :Cds_table_no from test30;

select :Cds_table_name :Cds_table_no from test40;

select :Cds_table_name :Cds_table_no from test50 where :Cds_table_name == "test60";  do not match

select :Cds_table_name :Cds_table_no from test60;

select :Cds_table_name :Cds_table_no from test70;

select :Cds_table_name :Cds_table_no from test80;

select :Cds_table_name :Cds_table_no from test180

select :Cds_table_name :Cds_table_no from test255;

select :Cds_table_name :Cds_table_no from test190;

select :Cds_table_name :Cds_table_no from test200;

select :Cds_table_name :Cds_table_no from test210

select :Cds_table_name :Cds_table_no from test220;

select :Cds_table_name :Cds_table_no from test230;

select :Cds_table_name :Cds_table_no from test240;

create table filler254 index (:Cds_table_name) allow delete buffer size 128;

select :Cds_table_name :Cds_table_no from filler254;

select :Cds_table_name :Cds_table_no from tables where :Cds_table_name == "filler254";

select :Cds_table_name :Cds_table_no from tables;

q
