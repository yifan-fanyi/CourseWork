-- using mySQL from https://paiza.io/projects/qkO_sstcsWRmaQe5VOYmbw?language=mysql
select SymptomID as 'most-self-reported symptom' from SYMPTOM 
  GROUP BY SymptomID 
  order BY COUNT(SymptomID) Desc 
  LIMIT 1;
