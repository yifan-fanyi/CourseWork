-- using mySQL from https://paiza.io/projects/qkO_sstcsWRmaQe5VOYmbw?language=mysql

CREATE TABLE EMPLOYEE (
  EmployeeID int NOT NULL PRIMARY KEY,
  Name varchar(30) NOT NULL,
  Tel varchar(15) NOT NULL,
  Email varchar(30),
  Office INT NOT NULL,
  Floor INT NOT NULL
);

Create table MEETING (
  MeetingID INT NOT NULL,
  MeetingRoom INT NOT NULL,
  Floor int NOT NULL,
  MeetingDate int NOT NULL,
  StartTime Int NOT NULL,
  Primary key (MeetingID)
);
  
create table ATTENDENTS (
  MeetingID int NOT NULL,
  EmployeeID int NOT NULL,
  CONSTRAINT meetingid_employeeid PRIMARY KEY (MeetingID, EmployeeID)
);

create table NOTIFICATION (
  NotificationID int NOT NULL,
  EmployeeID int NOT NULL,
  NotificationDate int NOT NULL,
  Type varchar(10) NOT NULL,
  PRIMARY KEY (NotificationID, EmployeeID)
);

create table SYMPTOM (
  RowID int NOT NULL,
  EmployeeID int NOT NULL,
  SymptomID int NOT NULL,
  DateReported INT NOT NULL,
  primary key (RowID)
);


Create table SCAN (
  ScanID int NOT NULL,
  EmployeeID int NOT NULL,
  ScanDate INT NOT NULL,
  ScanTime INT NOT NULL,
  Temperature INT NOT NULL,
  PRIMARY key (ScanID)
);

Create table TEST (
  TestID int NOT NULL,
  Location varchar(30) NOT NULL,
  EmployeeID int NOT NULL,
  TestDate int NOT NULL,
  TestTime int NOT NULL,
  TestResult varchar(20) NOT NULL,
  primary key (TestID)
);

create TABLE POSCASE (
  CaseID int NOT NULL,
  EmployeeID int NOT NULL,
  CaseDate INT NOT NULL,
  Resolution varchar(20),
  primary key (CaseID)
);

create table HEALTHSTATUS (
  RowID int NOT NULL,
  EmployeeID int NOT NULL,
  HSDate int NOT NULL,
  Status varchar(20) NOT NULL,
  Primary key (RowID)
);

-- add data
insert into EMPLOYEE VALues (1, 'Alex', '123-424-5353', 'alex@usc.edu', 1, 1);
insert into EMPLOYEE VALues (2, 'Alass', '163-424-5353', 'alass@usc.edu', 1, 1);
insert into EMPLOYEE VALues (3, 'Ben', '123-474-5353', 'ben@usc.edu', 1, 1);
insert into EMPLOYEE VALues (4, 'Joe', '123-424-7653', 'joe@usc.edu', 2, 1);
insert into EMPLOYEE VALues (5, 'Sam', '123-424-5233', 'sam@usc.edu', 2, 1);
insert into EMPLOYEE VALues (6, 'Tim', '123-434-5353', 'tim@usc.edu', 1, 2);
insert into EMPLOYEE VALues (7, 'King', '133-424-5353', 'king@usc.edu', 1, 2);
insert into EMPLOYEE VALues (8, 'Wong', '523-424-5353', 'wong@usc.edu', 1, 2);
insert into EMPLOYEE VALues (9, 'Jack', '923-424-5353', 'kack@usc.edu', 2, 3);
insert into EMPLOYEE VALues (10, 'Burger', '123-454-5353', 'burger@usc.edu', 1, 3);
insert into EMPLOYEE VALues (11, 'Butter', '143-424-5353', 'butter@usc.edu', 1, 4);
insert into EMPLOYEE VALues (12, 'Apple', '153-424-5453', 'apple@usc.edu', 2, 2);
insert into EMPLOYEE VALues (13, 'Peach', '123-466-5353', 'peach@usc.edu', 1, 3);
insert into EMPLOYEE VALues (14, 'Pen', '464-424-5353', 'pen@usc.edu', 1, 1);
insert into EMPLOYEE VALues (15, 'C', '163-534-5353', 'c@usc.edu', 1, 2);
insert into EMPLOYEE VALues (16, 'C#', '123-254-5353', 'c#@usc.edu', 1, 3);
insert into EMPLOYEE VALues (17, 'Python', '533-424-7653', 'python@usc.edu', 1, 4);
insert into EMPLOYEE VALues (18, 'Ruby', '123-424-5433', 'ruby@usc.edu', 1, 1);
insert into EMPLOYEE VALues (19, 'Bash', '123-434-6553', 'bash@usc.edu', 1, 2);
insert into EMPLOYEE VALues (20, 'Perl', '133-424-2263', 'perl@usc.edu', 1, 3);
insert into EMPLOYEE VALues (21, 'C++', '523-424-5324', 'c++@usc.edu', 1, 4);
insert into EMPLOYEE VALues (22, 'JavaScript', '923-234-5353', 'js@usc.edu', 1, 1);
insert into EMPLOYEE VALues (23, 'HTML', '123-454-5314', 'html@usc.edu', 1, 2);
insert into EMPLOYEE VALues (24, 'XMP', '143-424-5663', 'xmp@usc.edu', 1, 3);
insert into EMPLOYEE VALues (25, 'Swift', '153-424-5993', 'swift@usc.edu', 1, 4);
insert into EMPLOYEE VALues (26, 'Go', '123-466-5322', 'go@usc.edu', 1, 3);

insert into ATTENDENTS VALUES (1, 1);
insert into ATTENDENTS VALUES (1, 11);
insert into ATTENDENTS VALUES (1, 12);
insert into ATTENDENTS VALUES (1, 21);
insert into ATTENDENTS VALUES (2, 16);
insert into ATTENDENTS VALUES (2, 21);
insert into ATTENDENTS VALUES (2, 2);
insert into ATTENDENTS VALUES (2, 5);
insert into ATTENDENTS VALUES (3, 9);
insert into ATTENDENTS VALUES (3, 19);
insert into ATTENDENTS VALUES (3, 23);
insert into ATTENDENTS VALUES (4, 17);
insert into ATTENDENTS VALUES (4, 10);
insert into ATTENDENTS VALUES (5, 17);
insert into ATTENDENTS VALUES (5, 7);
insert into ATTENDENTS VALUES (5, 5);

insert into MEETING VALUES (1, 1, 1, 1, 8);
insert into MEETING VALUES (2, 2, 1, 2, 10);
insert into MEETING VALUES (3, 1, 2, 3, 13);
insert into MEETING VALUES (4, 1, 3, 2, 14);
insert into MEETING VALUES (5, 1, 4, 1, 17);

INSERT INTo SYMPTOM values (1, 1, 1, 1);
INSERT INTo SYMPTOM values (4, 1, 2, 3);
INSERT INTo SYMPTOM values (2, 4, 1, 3);
INSERT INTo SYMPTOM values (3, 10, 5, 2);
INSERT INTo SYMPTOM values (12, 10, 2, 5);
INSERT INTo SYMPTOM values (15, 10, 3, 3);

INSERT INTO SCAN values (1, 1, 1, 8, 98);
INSERT INTO SCAN values (2, 2, 1, 8, 98);
INSERT INTO SCAN values (3, 4, 1, 8, 98);

INSERT into TEST VALUES (1, 'company', 1, 1, 12, 'Negative');
INSERT into TEST VALUES (2, 'company', 2, 2, 12, 'Negative');
INSERT into TEST VALUES (3, 'clinic', 3, 1, 12, 'Positive');

INSERT into POSCASE values (1, 3, 1, 'left');

INSERT into HEALTHSTATUS VALUES (1, 1, 1, 'well');
INSERT into HEALTHSTATUS VALUES (2, 2, 1, 'sick');
INSERT into HEALTHSTATUS VALUES (3, 3, 1, 'sick');
INSERT into HEALTHSTATUS VALUES (4, 6, 1, 'sick');
INSERT into HEALTHSTATUS VALUES (5, 10, 1, 'well');
INSERT into HEALTHSTATUS VALUES (6, 13, 1, 'well');
INSERT into HEALTHSTATUS VALUES (7, 16, 1, 'well');
INSERT into HEALTHSTATUS VALUES (10, 17, 1, 'sick');
INSERT into HEALTHSTATUS VALUES (8, 17, 1, 'sick');
INSERT into HEALTHSTATUS VALUES (9, 17, 1, 'sick');