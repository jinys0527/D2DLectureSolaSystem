#include "pch.h"
#include "SimpleMathHelper.h"

namespace MYHelper
{
    // Copyright 2000 softSurfer, 2012 Dan Sunday
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.


// a Point is defined by its coordinates {int x, y;}
//===================================================================


// isLeft(): tests if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2  on the line
//            <0 for P2  right of the line
//    See: Algorithm 1 "Area of Triangles and Polygons"
 
    //===================================================================


    // cn_PnPoly(): crossing number test for a point in a polygon
    //      Input:   P = a point,
    //               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
    //      Return:  0 = outside, 1 = inside
    // This code is patterned after [Franklin, 2000]
    int cn_PnPoly(D2D1_POINT_2F P, std::vector<D2D1_POINT_2F> V, int n)
    {
        int    cn = 0;    // the  crossing number counter

        // loop through all edges of the polygon
        for (int i = 0; i < n; i++) {    // edge from V[i]  to V[i+1]
            if (((V[i].y <= P.y) && (V[i + 1].y > P.y))     // an upward crossing
                || ((V[i].y > P.y) && (V[i + 1].y <= P.y))) { // a downward crossing
                // compute  the actual edge-ray intersect x-coordinate
                float vt = (float)(P.y - V[i].y) / (V[i + 1].y - V[i].y);
                if (P.x < V[i].x + vt * (V[i + 1].x - V[i].x)) // P.x < intersect
                    ++cn;   // a valid crossing of y=P.y right of P.x
            }
        }

        printf("cn = %d\n", cn);

        return (cn & 1);    // 0 if even (out), and 1 if  odd (in)

    }
    //===================================================================


    // wn_PnPoly(): winding number test for a point in a polygon
    //      Input:   P = a point,
    //               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
    //      Return:  wn = the winding number (=0 only when P is outside)
    int  wn_PnPoly(D2D1_POINT_2F P, std::vector<D2D1_POINT_2F> V, int n)
    {
        int    wn = 0;    // the  winding number counter

        // loop through all edges of the polygon
        for (int i = 0; i < n; i++) {   // edge from V[i] to  V[i+1]
            if (V[i].y <= P.y) {          // start y <= P.y
                if (V[i + 1].y > P.y)      // an upward crossing
                    if (IsLeft(V[i], V[i + 1], P) > 0)  // P left of  edge
                        ++wn;            // have  a valid up intersect
            }
            else {                        // start y > P.y (no test needed)
                if (V[i + 1].y <= P.y)     // a downward crossing
                    if (IsLeft(V[i], V[i + 1], P) < 0)  // P right of  edge
                        --wn;            // have  a valid down intersect
            }
        }
        return wn;
    }
    //===================================================================
    //// 외접원안에 점이 들어오는지 확인

    bool is_circum(Triangle cur, int i, std::vector<Vector2F>& point)
    { 
        float ccw = (point[cur.b] - point[cur.a]).Cross(point[cur.c] - point[cur.a]);

        float adx = point[cur.a].x - point[i].x;
        float ady = point[cur.a].y - point[i].y;
        float bdx = point[cur.b].x - point[i].x;
        float bdy = point[cur.b].y - point[i].y;
        float cdx = point[cur.c].x - point[i].x;
        float cdy = point[cur.c].y - point[i].y;
        float bdxcdy = bdx * cdy, cdxbdy = cdx * bdy;
        float cdxady = cdx * ady, adxcdy = adx * cdy;
        float adxbdy = adx * bdy, bdxady = bdx * ady;
        float alift = adx * adx + ady * ady;
        float blift = bdx * bdx + bdy * bdy;
        float clift = cdx * cdx + cdy * cdy;


        float det = alift * (bdxcdy - cdxbdy)
            + blift * (cdxady - adxcdy)
            + clift * (adxbdy - bdxady);

        if (ccw > 0) return det >= 0;
        else return det <= 0;
    }

}