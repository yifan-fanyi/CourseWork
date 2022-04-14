create table LOC (name varchar(15) not null primary key,pos Point not null);

INSERT INTO LOC values ('Home', Point(-118.282447, 34.031506));
INSERT INTO LOC values ('NGate', Point(-118.283730, 34.022867));
INSERT INTO LOC values ('Leavey', Point(-118.283028, 34.021611));
INSERT INTO LOC values ('Pardee', Point(-118.282997, 34.020264));
INSERT INTO LOC values ('SGate', Point(-118.283363, 34.018800));
INSERT INTO LOC values ('Law', Point(-118.284850, 34.018967));
INSERT INTO LOC values ('Dog', Point(-118.286233, 34.018833));
INSERT INTO LOC values ('JSH', Point(-118.287317, 34.019669));
INSERT INTO LOC values ('OHE', Point(-118.288947, 34.020469));
INSERT INTO LOC values ('BookStore', Point(-118.286575, 34.020881));
INSERT INTO LOC values ('Trojan', Point(-118.285513, 34.020431));
INSERT INTO LOC values ('MSU', Point(-118.285308, 34.022758));
INSERT INTO LOC values ('Village', Point(-118.284722, 34.025272));

-- 4 NN
postgres=# SELECT name FROM LOC ORDER BY pos <-> Point(-118.282447, 34.031506) LIMIT 4 offset 1;



SELECT ST_ConcaveHull(ST_Union(POINT(110 170)'), 20), 1) As convexhull;
SELECT ST_ConvexHull(ST_Collect(r.pos)) AS the_geom FROM LOC AS r;

SELECT ST_AsText(ST_ConvexHull(ST_Collect(ST_GeomFromText('MULTIPOINT(50 5, 150 30, 50 10, 10 10)'))) );

SELECT ST_GeomFromText('CIRCULARSTRING(220268 150415,220227 150505,220227 150406)');

SELECT ST_AsText( ST_Collect( ST_GeomFromText('POINT(1. 2.)'), ST_GeomFromText('POINT(-2. 3.)') ));

SELECT ST_AsText( ST_Collect( ST_GeomFromText('POINT(1 2)'),
	ST_GeomFromText('POINT(-2 3)') ));