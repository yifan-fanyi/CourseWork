-- brew install postgresql
-- brew install postgis
-- to enable 'create extension postgis;'
-- copy all files in 
--      '/usr/local/Cellar/postgis/3.1.4_1/lib' 
-- to 
--      '/usr/local/Cellar/postgresql/14.0/lib'
-- copy all files in 
--      '/usr/local/Cellar/postgis/3.1.4_1/share/postgresql/extension'
-- to
--      '/usr/local/Cellar/postgresql/14.0/share/postgresql/extension'
--
-- start the postgre service in clt by
--      '/usr/local/opt/postgresql/bin/postgres -D /usr/local/var/postgres'
-- open another clt
--      'psql postgres'
--      'create extension postgis;'

create table LOC(name varchar(15) not null primary key, loc point not null);
INSERT INTO LOC values ('Home', Point(-118.282447, 34.031506)),('NGate', Point(-118.283730, 34.022867)),('Leavey', Point(-118.283028, 34.021611)),('Pardee', Point(-118.282997, 34.020264)),('SGate', Point(-118.283363, 34.018800)),('Law', Point(-118.284850, 34.018967)),('Dog', Point(-118.286233, 34.018833)),('JSH', Point(-118.287317, 34.019669)),('OHE', Point(-118.288947, 34.020469)),('BookStore', Point(-118.286575, 34.020881)),('Trojan', Point(-118.285513, 34.020431)),('MSU', Point(-118.285308, 34.022758)),('Village', Point(-118.284722, 34.025272));

-- convex hull
SELECT ST_AsText(ST_ConvexHull(ST_Collect(ST_GeomFromText('MULTIPOINT(-118.282447 34.031506,-118.283730 34.022867,-118.283028 34.021611,-118.282997 34.020264,-118.283363 34.018800,-118.284850 34.018967,-118.286233 34.018833,-118.287317 34.019669,-118.288947 34.020469,-118.286575 34.020881,-118.285513 34.020431,-118.285308 34.022758,-118.284722 34.025272)'))) );
-- POLYGON((-118.283363 34.0188,-118.286233 34.018833,-118.288947 34.020469,-118.282447 34.031506,-118.282997 34.020264,-118.283363 34.0188))
--   SGate  Dog  OHE  Home  Pardee  SGate

-- 4 nn
select name from LOC order by loc <-> Point(-118.282447, 34.031506) limit 4 offset 1;
-- Village  NGate  MSU  Leavey



