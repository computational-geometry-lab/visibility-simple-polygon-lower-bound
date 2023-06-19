# simple-polygon-lower-bound
This program is intend to verify that all of the visibilities are correct in limited visibility simple polygon.

How to execute:

chmod +x ./solve.sh

./solve.sh

Note that allviewpoints.txt stores the coordinates of guards and viewpoints, where A, B, C, ..., H represents g_A, g_B, g_C, ..., g_H, and rest of points are viewpoints (e.g. AB represents vp_{AB}, suppose to see g_A and g_B and does not see any other guards). We omitted the viewpoint sees empty set and the viewpoint only see one guard, which can be realiazable easily.

The SimplePolygonCoord.cpp will read the coord.txt and verify every viewpoint sees the guards that suppose to see and the guards that not suppose to see. If this viewpoint sees the guard, then the distance between the viewpoint and the guard must not greater than sight radius rho, which is 5, and the line segment between this viewpoint and the guard cannot be pierced. 
