// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <fbxsdk.h>


#include <CGAL/basic.h>

// standard includes
#include <iostream>
#include <fstream>
#include <cassert>


// includes for defining the Voronoi diagram adaptor
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Polygon_2.h>

//#include <>
//#include <CGAL/Delaunay_triangulation_traits_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Polygon_2.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

// typedefs for defining the adaptor
typedef CGAL::Exact_predicates_inexact_constructions_kernel                                    K;
typedef CGAL::Delaunay_triangulation_2<K>                                                                DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT>                                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT,AT,AP>                                                           VD;

// typedef for the result type of the point location
typedef AT::Site_2                    Site_2;
typedef AT::Point_2                   Point_2;

typedef VD::Locate_result                    Locate_result;
typedef VD::Vertex_handle                   Vertex_handle;
typedef VD::Face_handle                     Face_handle;
typedef VD::Halfedge_handle               Halfedge_handle;
typedef VD::Ccb_halfedge_circulator   Ccb_halfedge_circulator;


//use DT's dual to calculate VD
typedef CGAL::Cartesian<double> KernelCD;
typedef CGAL::Ray_2<KernelCD> Ray;
typedef CGAL::Line_2<KernelCD> Line;
typedef CGAL::Segment_2<KernelCD> Segment;
typedef std::list<Ray> RayList;
typedef std::list<Line> LineList;


typedef std::list<Segment> SegmentList;
typedef CGAL::Iso_rectangle_2<KernelCD> Rectangle;

typedef CGAL::Delaunay_triangulation_2<KernelCD>  DTtrait;
typedef DTtrait::Point                                                     DTPoint_2;


typedef CGAL::Point_2<KernelCD>                             CDPoint_2;
typedef CGAL::Ray_2<KernelCD>                               CDRay_2;

typedef CGAL::Polygon_2<K>                           CDPolygon_2;

//////////Polygon
//#include <CGAL/Cartesian.h>
//#include <CGAL/Polygon_2_algorithms.h>
//#include <CGAL/Polygon_2.h>
//typedef CGAL::Cartesian<double> Cd;
//typedef CGAL::Polygon_2<Cd> Polygon_2;
//typedef CGAL::Point_2<double> Point;


/////////Polygon




#include <fstream>




#include <iostream>
#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
