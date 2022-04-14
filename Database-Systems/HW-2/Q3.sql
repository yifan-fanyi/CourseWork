-- using mySQL from https://paiza.io/projects/qkO_sstcsWRmaQe5VOYmbw?language=mysql
-- use distinct to avoid duplicated report, each employee would only be ccounted once
select e.floor as sickestfloor from EMPLOYEE e 
where e.EmployeeID in (select distinct hs.EmployeeID from HEALTHSTATUS hs 
                       where hs.Status = 'sick')
group by sickestfloor
order by COUNT(sickestfloor) desc
limit 1;