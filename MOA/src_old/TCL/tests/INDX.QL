; test various index types - mixed, sparse, serial
drop t1;
drop t2;
drop t3;

create table t1 index (:Cds_table_name :Cds_table_no)
  allow delete buffer size 100;
create index sparse1 on t1 (:Cds_base_size sparse, :Cds_table_no);
; sparse create and alter should fail
create index serial1 on t1 (:Cds_count serial);
alter t1 index (:Cds_count serial);

create index many_key on t1 (:Cds_count, :Cds_table_name, :Cds_table_no,
 :Cds_base_size, :Cds_float_field, :Cds_index_name, :Cds_index_no,
 :Cds_index_key_seq, :Cds_key_changable, :Cds_unique_index, :Cds_index_appl_no,
 :Cds_index_field_no);   test of 12 key fields should succeed
drop index many_key from t1;

create index too_many on t1 (:Cds_count, :Cds_table_name, :Cds_table_no,
 :Cds_base_size, :Cds_float_field, :Cds_index_name, :Cds_index_no,
 :Cds_index_key_seq, :Cds_key_changable, :Cds_unique_index, :Cds_index_appl_no,
 :Cds_index_field_no,  :Cds_index_modifier);  should fail on too many segments
drop index too_many from t1;  should fail on not found

create table t2 index (:Cds_count :Cds_table_comment :Cds_table_no)
  allow keychange buffer size 300;  should fail on segment too long

create table t2 index (:Cds_count :Cds_table_no sparse)
  allow keychange buffer size 300;  should fail on sparse

create table t2 index (:Cds_count :Cds_table_no)
  allow keychange buffer size 300;  should succeed

create index mixed2 on t2 (:Cds_base_size :Cds_float_field :Cds_table_name)
 allow keychange;  should succeed;


create table t3 index (:Cds_count serial) allow keychange buffer size 100;

insert into t1 (:Cds_table_no :Cds_table_comment :Cds_count
:Cds_base_size :Cds_table_name :Cds_index_no) values
(20 comm20 20 20 tab20 20)
(18 comm18 18 18 tab18 18)
(16 comm16 16 16 tab16 16)
(14 comm14 14 14 tab14 14)
(12 comm12 12 12 tab12 12)
(10 comm10 10 10 tab10 10)
(8 comm8 8 8 tab8 8)
(6 comm6 6 6 tab6 6)
(4 comm4 4 4 tab4 4)
(2 comm2 2 2 tab2 2);

insert into t1 (:Cds_table_comment :Cds_table_no
:Cds_table_name :Cds_index_no) values
(comment19 19 tab19 19)
(comment17 17 tab17 17)
(comment15 15 tab15 15)
(comment13 13 tab13 13)
(comment11 11 tab11 11)
(comment9 9 tab9 9)
(comment7 7 tab7 7)
(comment5 5 tab5 5)
(comment3 3 tab3 3)
(comment1 1 tab1 1);

insert into t2 (:Cds_table_name :Cds_float_field :Cds_base_size
:Cds_index_no :Cds_table_comment :Cds_table_no :Cds_count) values
(tab2_15 15.2 15 15 comment_2_15 15 15)
(tab2_14 14.2 14 14 comment_2_14 14 14)
(tab2_13 13.2 13 13 comment_2_13 13 13)
(tab2_12 12.2 12 12 comment_2_12 12 12)
(tab2_11 11.2 11 11 comment_2_11 11 11)
(tab2_10 10.2 10 10 comment_2_10 10 10)
(tab2_9 9.2 9 9 comment_2_9 9 9)
(tab2_8 8.2 8 8 comment_2_8 8 8)
(tab2_7 7.2 7 7 comment_2_7 7 7)
(tab2_6 6.2 6 6 comment_2_6 6 6)
(tab2_5 5.2 5 5 comment_2_5 5 5)
(tab2_4 4.2 4 4 comment_2_4 4 4)
(tab2_3 3.2 3 3 comment_2_3 3 3)
(tab2_2 2.2 2 2 comment_2_2 2 2)
(tab2_1 1.2 1 1 comment_2_1 1 1)
(tab2_0 0.2 0 0 comment_2_0 0 0);


insert into t3 (:Cds_table_comment :Cds_table_name :Cds_table_no) values
("Serial key table" tab_3_15 15)
("Serial key table" tab_3_14 14)
("Serial key table" tab_3_13 13)
("Serial key table" tab_3_12 12)
("Serial key table" tab_3_11 11)
("Serial key table" tab_3_10 10)
("Serial key table" tab_3_9 9)
("Serial key table" tab_3_8 8)
("Serial key table" tab_3_7 7)
("Serial key table" tab_3_6 6)
("Serial key table" tab_3_5 5);

; The next inserts should fail on duplicate
insert into t3 (:Cds_table_comment :Cds_table_name :Cds_table_no
  :Cds_count) values
("Serial key table" tab_3_4 4  4)
("Serial key table" tab_3_3 3  3)
("Serial key table" tab_3_2 2  2)
("Serial key table" tab_3_1 1  1);   these should fail on duplicate

select * from t1;
select * from t2;
select * from t3;

select :Cds_count :Cds_base_size from t1;
select :Cds_base_size :Cds_table_name from t1
  sort by sparse1 descending;

select :Cds_table_name :Cds_base_size from t2
  sort by mixed2 descending;

select :Cds_count :Cds_table_no from t3 order descending;
;
; Test integer key with where clause, find last.
declare ind3 cursor for select :Cds_count from t3 where :Cds_count > 5;
fetch last ind3;
fetch prior ind3;
fetch first ind3;
fetch last ind3;
fetch next ind3;
fetch next ind3;
close ind3;
declare ind3 cursor for select :Cds_count from t3 where :Cds_count <= 5;
fetch last ind3;
fetch prior ind3;
fetch first ind3;
fetch last ind3;
fetch next ind3;
fetch next ind3;
close ind3;

; test deletes on sparse index
declare ind1 cursor for select :Cds_table_no from t1
  where :Cds_table_no > 10;

fetch first ind1;     11
fetch next ind1;      12
fetch next ind1 with lock; 13
delete from t1 where current of ind1;
fetch last ind1;
fetch prior ind1 with lock;
delete from t1 where current of ind1;
close ind1;

delete from t1 where :Cds_table_no > 5 and :Cds_table_no < 11;


; test find_n with holey key
declare ind2 cursor for select :Cds_table_name :Cds_table_no from t2
  where :Cds_table_no < 12;

fetch 4 from ind2;
fetch 4 from ind2;
fetch 4 from ind2;
fetch 4 from ind2;
fetch 4 -4 from ind2;
fetch 4 from ind2;
fetch first ind2;
fetch last ind2;
fetch -5 ind2;
fetch -5 ind2;
fetch -5 ind2;
fetch -5 ind2;
fetch 5 ind2;
close ind2;

declare ind2 cursor for select :Cds_table_name :Cds_table_no from t2
  where :Cds_table_no < 12 order descending;

fetch 4 from ind2;
fetch 4 from ind2;
fetch 4 from ind2;
fetch 4 from ind2;
fetch 4 from ind2;
fetch first ind2;
fetch last ind2;
fetch -5 ind2;
fetch -5 ind2;
fetch -5 ind2;
fetch -5 ind2;
fetch 5 ind2;
close ind2;
;
; test insert of duplicate record with composite type key
insert into t2 (:Cds_table_name :Cds_float_field :Cds_base_size
:Cds_index_no :Cds_table_comment :Cds_table_no :Cds_count) values
(tab2_11 11.2 11 11 comment_2_11 11 11);

insert into t2 (:Cds_table_name :Cds_float_field :Cds_base_size
:Cds_index_no :Cds_table_comment :Cds_table_no :Cds_count) values
(tab2_6 6.2 6 6 comment_2_6 6 6);

alter table t2 index (:Cds_count :Cds_table_no sparse); should fail on sparse

show indexes on t2;

; test of update_all on current index
update t2 set :Cds_table_name = "updated" where :Cds_table_no = 1;
update t2 set :Cds_table_name = "tab2_1" where :Cds_table_no = 1;

update t1 set :Cds_base_size = 18 where :Cds_index_no == 17;
select unique :Cds_base_size from t1 order by sparse1;
select unique :Cds_base_size from t1 order by sparse1 descending;

delete from t1;   testing delete
delete from t2;
delete from t3;

; clean up, all done
drop t1;
drop t2;
drop t3;

