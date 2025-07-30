select :Cds_table_name from tables;

create table tj keys(:Cds_table_no, :Cds_index_no, :Cds_index_key_seq)
 allow delete buffer size 140;

create table ta keys(:Cds_num4, :Cds_count)
buffer size 140;

create table tb keys(:Cds_count)
buffer size 140;

create table tc keys(:Cds_num2, :Cds_date)
buffer size 140;

create table td keys(:Cds_table_no, :Cds_index_no, :Cds_index_key_seq)
 allow delete buffer size 140;

create table te keys(:Cds_table_no, :Cds_index_no, :Cds_index_key_seq)
 allow delete buffer size 140;

select from indexes dump into /tmp/dumpfile;
insert into tj from /tmp/dumpfile;
! rm /tmp/dumpfile

select :Cds_table_name, :Cds_index_field_name
from tables, tj as i
where tables.:Cds_table_no == i.:Cds_table_no;

select :Cds_table_name, :Cds_index_field_name
from tables preserve, tj as i
where tables.:Cds_table_no == i.:Cds_table_no;

delete from tj where :Cds_table_no > 1 && :Cds_table_no < 4;

select :Cds_table_name, :Cds_index_field_name
from tables, tj as i
where tables.:Cds_table_no == i.:Cds_table_no;

select :Cds_table_name, :Cds_index_field_name
from tables preserve, tj as i
where tables.:Cds_table_no == i.:Cds_table_no;

drop table te;
drop table td

select :Cds_table_name, :Cds_index_field_name
from tables, tj as i
where tables.:Cds_table_no == i.:Cds_table_no;

select :Cds_table_name, :Cds_index_field_name
from tables preserve, tj as i
where tables.:Cds_table_no == i.:Cds_table_no;

declare x cursor for select  :Cds_table_name, :Cds_index_field_name
from tables, tj as i
where tables.:Cds_table_no == i.:Cds_table_no;
fetch count(:Cds_count) from x;

close x;
drop table tj;
drop table ta;
drop table tb;
drop table tc;
quit;
