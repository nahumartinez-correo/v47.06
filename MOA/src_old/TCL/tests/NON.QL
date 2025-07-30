;  Do some testing of non-unique indexes
;  Start with problems related to fetch again, fetch last
set page length 0;
create table nonu index (:Cds_table_no) allow delete allow keychange
buffer size 200;

; add some non-unique indexes
create index non1 on nonu (:Cds_table_name) allow keychange;
create index non2 on nonu (:Cds_table_name :Cds_count) allow keychange;
create index non3 on nonu (:Cds_count) allow keychange;

insert into nonu (:Cds_table_no :Cds_table_name :Cds_count) values
(1 hhhh 1) (2 hhhh 1) (3 hhhh 1) (4 hhhh 1) (5 hhhh 1);

declare c1 cursor for select * from nonu order by primary;
declare c2 cursor for select * from nonu order by non1;
declare c3 cursor for select * from nonu order by non2 descending;
declare c4 cursor for select * from nonu order by non3;

fetch first c1;
fetch first c2;
fetch first c3;
fetch first c4;
fetch last c1;
fetch last c2;
fetch last c3;
fetch last c4;
fetch prior c1;
fetch prior c2;
fetch prior c3;
fetch prior c4;
fetch again c1;
fetch again c2;
fetch again c3;
fetch again c4;

; now place some records on both sides of the existing
insert into nonu (:Cds_table_no :Cds_table_name :Cds_count) values
(0 aaaa 0) (6 aaaa 2) (7 bbbb 0) (8 xxxx 0) (9 xxxx 2);

close c1;
close c2;
close c3;
close c4;

declare c1 cursor for select * from nonu where :Cds_table_no > 2
&& :Cds_table_no < 8 order by primary;
declare c2 cursor for select * from nonu where :Cds_table_name == "hhhh" order by non1;
declare c3 cursor for select * from nonu where :Cds_count == 0 order by non2;
declare c4 cursor for select * from nonu where :Cds_count == 1
order by non3 descending;

fetch first c1;
fetch first c2;
fetch first c3;
fetch first c4;
fetch last c1;
fetch last c2;
fetch last c3;
fetch last c4;
fetch prior c1;
fetch prior c2;
fetch prior c3;
fetch prior c4;
fetch again c1;
fetch again c2;
fetch again c3;
fetch again c4;

close c1;
close c2;
close c3;
close c4;
select * from nonu order by non1;
select * from nonu order by non2;
drop nonu;
q;

