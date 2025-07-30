
; script to test projections, aggregates and alike

create table proj index (:Cds_table_name) allow delete allow keychange buffer size 75;

insert into proj (:Cds_table_name :Cds_table_no :Cds_count :Cds_float_field :system_date)
values
(rec1 1 33 12.5 19901001)
(rec2 2 6 17.14 19901002)
(rec3 3 0 8832454.1 19901003)
(rec4 4 345 1.0 19901004)
(rec5 5 543 567.33 19901005)
(rec6 6 17 92.00 19901006)
(rec7 7 13 0.5 19901007)
(rec8 8 31 22.95 19901008);

insert into proj (:Cds_table_name :Cds_table_no :system_date)
values
(rec9 9 19901009);

insert into proj (:Cds_table_name :Cds_table_no :Cds_float_field :system_date)
values
(rec10 10 2.5 19901010);

insert into proj (:Cds_table_name :Cds_table_no :Cds_count :system_date)
values
(rec11 11 3  19901011);

insert into proj (:Cds_table_name :Cds_table_no :Cds_count :Cds_float_field :system_date)
values
(rec12 12 7 137.5 19901012);

; test normal projections and projections on cursors

select 1 from proj;

declare cur cursor for select :Cds_table_name as "cursor name",
:Cds_table_no as "cursor number" from proj;

fetch cur;

select :Cds_table_no, :Cds_count width 12 :system_date from proj;

select :Cds_table_name as "Name", "junk", :system_date as "Date" width 12 
from proj;

fetch cur;  tests the projection list changes

select  :system_date :Cds_table_no :system_date :Cds_table_name :system_date
from proj;

fetch last from cur;

; test some aggregates

select count(:Cds_table_name), :Cds_table_name from proj;

fetch count(:Cds_float_field), max(:Cds_float_field) from cur;

fetch first from cur;

fetch count(:Cds_float_field), max(:Cds_float_field) from cur;

select count(:Cds_count), min(:Cds_count), max(:Cds_count), sum(:Cds_count),
avg(:Cds_count) from proj where :Cds_count > 10;

fetch first from cur;

; should test with some error states some time

; test update_all and delete_all features

update proj set :Cds_count = 1 where :Cds_count > 10;

select :Cds_table_name :Cds_count from proj;

delete from proj where :Cds_count > 10;

select :Cds_table_name :Cds_count from proj;

fetch first cur;
fetch next cur;
fetch next cur;
fetch next cur with lock;

update proj set :Cds_table_no = 50 where :Cds_table_no < 5;

select :Cds_table_name :Cds_table_no from proj;

fetch next cur with lock;

delete from proj where :Cds_table_no < 8;

close cur;

delete from proj;

select :Cds_table_name :Cds_table_no from proj;

drop proj;

quit;



