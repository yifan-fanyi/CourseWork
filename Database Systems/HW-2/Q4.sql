-- using mySQL from https://paiza.io/projects/qkO_sstcsWRmaQe5VOYmbw?language=mysql
-- specify the start date and end date
-- the result would include both the start data and end data
set @StartDate = 0;
set @EndDate = 4;

select Count(s.ScanID) as '#Scan' from SCAN s where s.ScanDate >= @StartDate and s.ScanDate <= @EndDate;
select Count(s.TestID) as '#Test' from TEST s where s.TestDate >= @StartDate and s.TestDate <= @EndDate;
select Count(s.RowID) as '#SelfReport' from SYMPTOM s where s.DateReported >= @StartDate and s.DateReported <= @EndDate;
select Count(s.TestID) as '#TestPositive' from TEST s where s.TestDate >= @StartDate and s.TestDate <= @EndDate and s.TestResult='Positive';

