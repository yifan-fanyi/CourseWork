-- using mySQL from https://paiza.io/projects/qkO_sstcsWRmaQe5VOYmbw?language=mysql
-- To: list employees has some specified symptons, 
-- for example we want to know employee who has both symptom#1 and symptom#2
-- <SYMPTOM.SymptomID, SYMPTOM.EmployeeID> divide <SS.SymptomID>
-- return EmployeeID
Create table SS (
  SymptomID int not null primary key
);

INSERT INTo SS values (1);
INSERT INTo SS values (2);

select s.SymptomID, s.EmployeeID from SYMPTOM s;
select * from SS;

select distinct e1.EmployeeID as 'Employee has all specified symptoms' 
from SYMPTOM e1
where Not exists (
    select p.SymptomID 
    from SS p 
    where p.SymptomID not in (
       select e2.SymptomID
       from SYMPTOM e2 
       where e2.EmployeeID = e1.EmployeeID 
   )
);

   