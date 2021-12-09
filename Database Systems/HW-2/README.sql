-- The code uses mySQL 
-- Test using the online mySQL editor 
-- url to the editor I used ->  https://paiza.io/projects/qkO_sstcsWRmaQe5VOYmbw?language=mysql

-- Time and data are assume as int, same flavor as time.time()
-- Q1
-- EMPLOYEE and MEETING is connected by ATTENDENTS to avoid redundency

-- Q3
-- use 'distinct' to avoid duplicated report, each employee would only be ccounted once

-- Q4
-- specify the start date and end date
-- the result would include both the start data and end data

-- Q5 
-- To: list employees has some specified symptoms, 
-- for example we want to know employee who has both symptom#1 and symptom#2
-- a new table SS which lists the symptoms we are interested in is built
-- <SYMPTOM.SymptomID, SYMPTOM.EmployeeID> divide <SS.SymptomID>
-- return EmployeeID