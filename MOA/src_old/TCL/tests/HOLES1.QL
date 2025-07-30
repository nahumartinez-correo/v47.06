drop table t;
create table t keys(:Cds_count serial) buffer size 100 allow delete;
create index ts on t (:Cds_index_name, :Cds_table_name);
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
(aa,bbh10)
(aa,bbi11)
(bb,bbj12)
(bb,bbk13)
(bb,bbl14)
(bb,bbm15)
(bb,bbn16)
(bb,bbo17)
(bb,bbp18)
(bb,bbq19)
(bb,bbr20)
(bb,ab)
(bb,bb3)
(cc,ba4)
(cc,bc5)
(cc,bd6)
(cc,be7)
(cc,bf8)
(cc,bg9)
(cc,bh10)
(cc,bi11)
(cc,bj12)
(cc,bk13)
(cc,bl14)
(dd,bm15)
(dd,bn16)
(dd,bo17)
(dd,bp18)
(dd,bq19)
(dd,br20)
(dd,b)
(dd,b3)
(dd,a4)
(dd,c5)
(dd,d6)
(ee,e7)
(ee,f8)
(ee,g9)
(ee,h10)
(ee,i11)
(ee,j12)
(ee,k13)
(ee,l14)
(ee,m15)
(ee,n16)
(ee,o17)
(ff,p18)
(ff,q19)
(ff,r20)
(ff,r21)
(ff,3)
(ff,4)
(ff,5)
(ff,6)
(ff,7)
(ff,8)
(ff,9)
(gg,10)
(gg,11)
(gg,12)
(gg,13)
(gg,14)
(gg,15)
(gg,16)
(gg,17)
(gg,18)
(gg,19)
(gg,20)
(gg,caz21);
select from t where :Cds_table_name smatch "caz*" order by ts;
select from t where :Cds_table_name smatch "caz*";
select from t where :Cds_index_name smatch "g*" order by ts;
select from t where :Cds_count bitsoff 7;
;
;    Tests added 2/15/94 for additional holey key testing
;
drop table hole;
create table hole index (common:Cds_table_name :Cds_table_no)
allow delete allow keychange buffer size 200;
insert into hole (:Cds_table_name :Cds_table_no :Cds_float_field,
:Cds_date, :Cds_count, :Cds_owner) values
(tab1, 1, 2.5, 19940201, 123, me)
(tab1, 2, 12.5, 19940202, 999, me)
(tab1, 3, 22.5, 19940204, 123, me)
(tab1, 4, 32.5, 19940206, 888, me)
(tab2, 1, 42.5, 19940208, 123, me)
(tab2, 2, 52.5, 19940201, 777, me)
(tab2, 3, 62.5, 19940211, 123, me)
(tab2, 4, 72.5, 19940221, 666, me)
(tab3, 6, 82.5, 19940203, 123, me)
(tab3, 7, 92.5, 19940205, 555, me)
(tab4, 2, 92.5, 19940207, 123, you)
(tab4, 3, 82.5, 19940209, 444, me)
(tab4, 5, 72.5, 19940201, 123, me)
(tab4, 6, 62.5, 19940201, 123, me)
(tab4, 7, 52.5, 19940211, 333, me)
(tab4, 8, 42.5, 19940221, 123, me)
(tab5, 9, 32.5, 19940201, 222, me)
(tab6, 2, 22.5, 19940211, 123, me)
(tab6, 3, 12.5, 19940203, 111, me)
(tab6, 4, 2.5, 19940206, 100, me)
(tab6, 5, 12.5, 19940211, 123, me)
(tab6, 7, 22.5, 19940207, 111, you)
(tab6, 8, 32.5, 19940221, 123, me)
(tab7, 4, 42.5, 19940201, 222, me)
(tab7, 5, 52.5, 19940201, 123, me)
(tab7, 6, 62.5, 19940208, 333, me)
(tab7, 7, 72.5, 19940201, 123, me)
(tab7, 8, 82.5, 19940211, 444, me)
(tab7, 9, 92.5, 19940210, 123, me)
(tab8, 9, 82.5, 19940209, 555, you)
(tab8, 8, 72.5, 19940208, 123, me)
(tab8, 7, 62.5, 19940207, 666, me)
(tab8, 6, 52.5, 19940206, 123, me)
(tab8, 5, 42.5, 19940205, 777, me)
(tab8, 4, 32.5, 19940204, 123, me)
(tab8, 3, 22.5, 19940203, 888, me)
(tab8, 2, 12.5, 19940202, 123, you)
(tab9, 1, 2.5, 19940201, 999, me);
; create alternate indexes for testing
create index hole1 on hole (:Cds_table_name, :Cds_count, :Cds_date)
allow keychange;

create index hole2 on hole (:Cds_owner, :Cds_date, :Cds_table_no, :Cds_float_field)
allow keychange;

create unique index hole3 on hole 
(:Cds_date, :Cds_owner, :Cds_table_name, :Cds_count, :Cds_table_no)
allow keychange;
; selects for testing holey keys....
select * from hole;
; test with holey and non-holey access
select * from hole where :Cds_table_name == "tab7";
select * from hole where :Cds_table_name == "tab7" order by hole1;
select * from hole where :Cds_table_name == "tab7" order by hole2;
select * from hole where :Cds_table_name == "tab7" order by hole3;
select * from hole where :Cds_table_name == "tab7"
order by hole3 descending;
;
;  Another test of holey access with equality on single value
;
select * from hole where :Cds_count == 111;
select * from hole where :Cds_count == 111 order by hole1;
select * from hole where :Cds_count == 111 order by hole1 descending;
select * from hole where :Cds_count == 111 order by hole2;
select * from hole where :Cds_count == 111 order by hole3;
select * from hole where :Cds_count == 111 order by hole3 descending;
;
;  Try multiple fields on equality
;
select * from hole where :Cds_owner == "me" && :Cds_table_no == 2;
select * from hole where :Cds_owner == "me" && :Cds_table_no == 2
order by hole1;
select * from hole where :Cds_owner == "me" && :Cds_table_no == 2
order by hole2;
select * from hole where :Cds_owner == "me" && :Cds_table_no == 2
order by hole2 descending;
select * from hole where :Cds_owner == "me" && :Cds_table_no == 2
order by hole3;
select * from hole where :Cds_owner == "me" && :Cds_table_no == 2
order by hole3 descending;
;
;  Try single file with < or >
;
select * from hole where :Cds_count > 777;
select * from hole where :Cds_count > 777 order by hole1;
select * from hole where :Cds_count > 777 order by hole1 descending;
select * from hole where :Cds_count > 777 order by hole2;
select * from hole where :Cds_count > 777 order by hole3;
select * from hole where :Cds_count > 777 order by hole3 descending;
;
; now try <  NOTE that things like match not yet tried
;
select * from hole where :Cds_date < 19940204;
select * from hole where :Cds_date < 19940204 order by hole1;
select * from hole where :Cds_date < 19940204 order by hole1
descending;
select * from hole where :Cds_date < 19940204 order by hole2;
select * from hole where :Cds_date < 19940204 order by hole2
descending;
select * from hole where :Cds_date < 19940204 order by hole3;
select * from hole where :Cds_date < 19940204 order by hole3
descending;
;
; Now lets try multiple fields with mixed comparators
;
select * from hole where :Cds_owner > "me" && :Cds_count < 130;
select * from hole where :Cds_owner > "me" && :Cds_count < 130
order by hole1;
select * from hole where :Cds_owner > "me" && :Cds_count < 130
order by hole1 descending;
select * from hole where :Cds_owner > "me" && :Cds_count < 130
order by hole2;
select * from hole where :Cds_owner > "me" && :Cds_count < 130
order by hole3;
select * from hole where :Cds_owner > "me" && :Cds_count < 130
order by hole3 descending;
;
;  Another variation
;
select * from hole where :Cds_owner == "me" && :Cds_table_name > "tab6" &&
:Cds_count < 678;
select * from hole where :Cds_owner == "me" && :Cds_table_name > "tab6" &&
:Cds_count < 678 order by hole1;
select * from hole where :Cds_owner == "me" && :Cds_table_name > "tab6" &&
:Cds_count < 678 order by hole1 descending;
select * from hole where :Cds_owner == "me" && :Cds_table_name > "tab6" &&
:Cds_count < 678 order by hole2;
select * from hole where :Cds_owner == "me" && :Cds_table_name > "tab6" &&
:Cds_count < 678 order by hole2 descending;
select * from hole where :Cds_owner == "me" && :Cds_table_name > "tab6" &&
:Cds_count < 678 order by hole3;
select * from hole where :Cds_owner == "me" && :Cds_table_name > "tab6" &&
:Cds_count < 678 order by hole3 descending;
;
;
;   Finally, these are selects that failed in random testing
;
select * from hole where :Cds_table_no == 4;
select * from hole where :Cds_table_no == 4 order by hole2 descending;
;
;
select * from hole where :Cds_count == 123 order by hole1 descending;
select * from hole where :Cds_count == 123;
;
;
select :Cds_table_name :Cds_date :Cds_owner from hole
where :Cds_date == 19940201 && :Cds_owner == "me" && :Cds_count == 123;
select :Cds_table_name :Cds_date :Cds_owner from hole
where :Cds_date == 19940201 && :Cds_owner == "me"
&& :Cds_count == 123 order by hole3;
select :Cds_table_name :Cds_date :Cds_owner from hole
where :Cds_date == 19940201 && :Cds_owner == "me"
&& :Cds_count == 123 order by hole3 descending;
;
;
select * from hole where :Cds_date = 19940210 order by hole2;
select * from hole where :Cds_date = 19940210 order by hole2 descending;
;
;
select * from hole where :Cds_date > 19940205 and :Cds_table_no == 6
order by hole2;
select * from hole where :Cds_date > 19940205 and :Cds_table_no == 6;
;
;    Basically same as last one
;
select * from hole where :Cds_date > 19930101 and :Cds_table_no == 6
order by hole2;
select * from hole where :Cds_date > 19930101 and :Cds_table_no == 6;
