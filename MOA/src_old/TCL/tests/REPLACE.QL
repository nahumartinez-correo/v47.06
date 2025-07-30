drop table reptest;
create table reptest keys (:Cds_num2) allow delete allow keychange
buffer size 60;
create index two on reptest (:Cds_table_name) allow keychange;
insert into reptest (:Cds_num2, :Cds_table_name) values (1, aaa);
declare r cursor for select from reptest;
fetch r with lock;
update reptest set :Cds_num2 = 2, :Cds_table_name = "bbb"
where current of r;
close r;
