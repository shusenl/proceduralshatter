// fbxShatterApp.cpp : Defines the entry point for the console application.
// Prototype 4-21-2010 by Shusen Liu, University of Utah

#include "stdafx.h"

extern "C" bool SaveScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat, bool pEmbedMedia);
extern "C" bool LoadScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename);
bool isPointInPolygon(CDPoint_2 p, CDPolygon_2 polygon)
{
	CDPolygon_2::Vertex_iterator pit = polygon.vertices_begin();
	CDPolygon_2::Vertex_iterator pitend = polygon.vertices_end();
	switch (CGAL::bounded_side_2(pit, pitend, p, KernelCD()))
	{
	case CGAL::ON_BOUNDED_SIDE:
		return true;
		break;
	case CGAL::ON_BOUNDARY:
		return true;
		break;
	case CGAL::ON_UNBOUNDED_SIDE:
		return false;
		break;
	default:
		return false;
	}
}
bool getRaySegmentsIntersect(CDRay_2 ray, std::vector<CDSegment_2> &segmentList, 
							 const CDPoint_2 *&intersectPoint, std::vector<CDSegment_2>::iterator &segit, CGAL::Object &obj)
{
	//CDSegment_2::iterator;
	//segit = segmentList.begin();
	//CDPoint_2 intersectPoint;
	//CGAL::Object obj;

	for(; segit != segmentList.end(); ++(segit))
	{
		obj = intersection(ray, *segit);
		if ( intersectPoint = CGAL::object_cast<CDPoint_2>(&obj)) 
		{
			std::cout<< "Intersect At: "<< *intersectPoint <<"\n\n"<< std::endl;
			//KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
			//polygonPoints.push_back(P);
			//firstIntersectSegIt = segit;
			return true;
			break;
		}
	}
	return false;
}

bool getSegmentSegmentsIntersect(CDSegment_2 sray, std::vector<CDSegment_2> &segmentList, 
							 const CDPoint_2 *&intersectPoint, std::vector<CDSegment_2>::iterator &segit, CGAL::Object &obj)
{
	//CDSegment_2::iterator;
	//segit = segmentList.begin();
	//CDPoint_2 intersectPoint;
	//CGAL::Object obj;

	for(; segit != segmentList.end(); ++(segit))
	{
		obj = intersection(sray, *segit);
		if ( intersectPoint = CGAL::object_cast<CDPoint_2>(&obj)) 
		{
			std::cout<< "Intersect At: "<< *intersectPoint <<"\n\n"<< std::endl;
			//KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
			//polygonPoints.push_back(P);
			//firstIntersectSegIt = segit;
			return true;
			break;
		}
	}
	return false;
}
void shiftToFirstUnboundEdge(VD::Face::Ccb_halfedge_circulator &c_halfedge)
{
	bool founded = false;
	VD::Face::Ccb_halfedge_circulator c_temp;
	while( !founded )
	{ 
		c_temp = c_halfedge++;
		if(c_temp->is_unbounded () && c_temp->has_source() //when you only have two unbounded edge in a polygon, this condition is needed
			&& (++c_temp)->is_unbounded())
			founded = true;
		//c_halfedge++;
	}
	c_halfedge--;
	assert(c_temp->is_unbounded());
	c_temp--;
	assert(c_temp->is_unbounded());

	//if(c_halfedge->is_unbounded())
	//	return (--c_halfedge);
	//else
	//return c_halfedge;

}


int _tmain(int argc, _TCHAR* argv[])
{
	
#pragma region DonnotCare
    KFbxSdkManager* SdkManager = NULL;
    KFbxScene* Scene = NULL, *outPutScene = NULL;
	KFbxNode* Node = NULL, *outPutNode = NULL;

    bool Result;

    // Prepare the FBX SDK.
	SdkManager = KFbxSdkManager::Create();

	Scene = KFbxScene::Create(SdkManager, "");
	outPutScene = KFbxScene::Create(SdkManager, "");

	Node = KFbxNode::Create(SdkManager, "");

	//IImporter = KFbxImporter::Create(lSdkManager, "");
	//IExporter = KFbxExporter::Create(ISdkManager, "");


    // Load the scene.
	Result = LoadScene(SdkManager,Scene, "toBeShatter.fbx");
	std::cout<<"loading "<< (Result ? "Successful!" : "Failed!") <<std::endl;
	

	// Get the geometry from the scene
	std::cout<<"\n Geometry count: " << Scene->GetGeometryCount() << std::endl;
	
	KFbxMesh *mesh, *outputMesh;
	Node = Scene->GetRootNode();

	std::cout<<"\n Node Child count: "<<Node->GetChildCount() << std::endl;

	Node = Node->GetChild(0);
	mesh = Node->GetMesh();

	int PolySize = mesh->GetPolygonSize (0);
	std::cout<<"\n Polygon in the mesh: "<<mesh->GetPolygonCount ()<< std::endl;
	std::cout<<"\n Vertex in the polygon: "<<PolySize << std::endl;


	//how to get polygon vertex index in certain mesh
	int Poly_index = mesh->GetPolygonVertex(0, 0);
	//get polygon vertex array
	int *p_Mesh = mesh->GetPolygonVertices();
	KFbxVector4* p_CP = mesh->GetControlPoints();

	std::cout << p_CP[0][0] << " " << p_CP[0][1] << " " << p_CP[0][2] << std::endl;
 	std::cout << p_CP[1][0] << " " << p_CP[1][1] << " " << p_CP[1][2] << std::endl;
	std::cout << p_CP[2][0] << " " << p_CP[2][1] << " " << p_CP[2][2] << std::endl;
	std::cout << p_CP[3][0] << " " << p_CP[3][1] << " " << p_CP[3][2] << std::endl;

	//XXXXXXXXXXXXXXXXXXXXXXXXX
	Fbx fbx;
	fbx.LoadScene("toBeShatter.fbx");



	/////////////CGAL/////////////////



	std::vector<CDSegment_2> segmentList;
	segmentList.push_back(  CDSegment_2(CDPoint_2(p_CP[0][0], p_CP[0][2]), 	CDPoint_2(p_CP[2][0], p_CP[2][2]))  );

	segmentList.push_back(  CDSegment_2(CDPoint_2(p_CP[2][0], p_CP[2][2]), 	CDPoint_2(p_CP[3][0], p_CP[3][2]))  );

	segmentList.push_back(  CDSegment_2(CDPoint_2(p_CP[3][0], p_CP[3][2]), 	CDPoint_2(p_CP[1][0], p_CP[1][2]))  );

	segmentList.push_back(  CDSegment_2(CDPoint_2(p_CP[1][0], p_CP[1][2]),	CDPoint_2(p_CP[0][0], p_CP[0][2]))  );
	
	CDIsoRectangle_2 isoRect(CDPoint_2(p_CP[3][0], p_CP[3][2]),  CDPoint_2(p_CP[0][0], p_CP[0][2]));
	CDPoint_2 pointarray[]={CDPoint_2(p_CP[0][0], p_CP[0][2]), CDPoint_2(p_CP[2][0], p_CP[2][2]), CDPoint_2(p_CP[3][0], p_CP[3][2]), CDPoint_2(p_CP[1][0], p_CP[1][2]) };
	CDPolygon_2 pgn(pointarray, pointarray+4);
	//pgn(pointarray, pointarray+4);

	//const CDPoint_2 p1=CDPoint_2(p_CP[0][0], p_CP[0][2]);
	//const CDPoint_2 p2=CDPoint_2(p_CP[2][0], p_CP[2][2]);
	//const CDPoint_2 p3=CDPoint_2(p_CP[3][0], p_CP[3][2]);
	//const CDPoint_2 p4=CDPoint_2(p_CP[1][0], p_CP[1][2]);

	//pgn.push_back(p1);
	//pgn.push_back(p2);
	//pgn.push_back(p3);
	//pgn.push_back(p4);

	std::vector<CDSegment_2>::iterator itt = segmentList.begin();
	std::cout<< *itt << std::endl;
	std::cout<< *(++itt) << std::endl;
	std::cout<< *(++itt) << std::endl;

	VD vd;
	Site_2 t1(0.5, 2.3);
	Site_2 t2(2.1, 0);
	Site_2 t3(-1.1, 0.1);
	Site_2 t4(0,-2.7);
	Site_2 t5(2.9,0);
	vd.insert(t1);
	vd.insert(t2);
	vd.insert(t3);
	vd.insert(t4);
	vd.insert(t5);
	vd.insert(Site_2(2.66, 2.1));
	vd.insert(Site_2(4.2, -3.1));
	vd.insert(Site_2(7.4, 1.1));
	vd.insert(Site_2(-1.3, 2.1));
	vd.insert(Site_2(-5.3, -4.1));
	vd.insert(Site_2(-3.9, -0.1));
	vd.insert(Site_2(3.66, 5.1));
	vd.insert(Site_2(7.2, -4.1));
	vd.insert(Site_2(6.4, 3.1));
	vd.insert(Site_2(-1.3, 5.61));
	vd.insert(Site_2(-6.3, -4.1));
	vd.insert(Site_2(2.9, -1.1));
	vd.insert(Site_2(-3.66, 5.1));
	vd.insert(Site_2(7.2, 4.1));
	vd.insert(Site_2(-6.4, 3.1));
	vd.insert(Site_2(1.3, 5.61));
	vd.insert(Site_2(-6.3, 4.1));
	vd.insert(Site_2(-2.9, -1.1));

	vd.insert(Site_2(-5.03, -4.91));
	vd.insert(Site_2(-3.11, -0.31));
	vd.insert(Site_2(3.06, 5.9));
	vd.insert(Site_2(7.76, -4.89));
	vd.insert(Site_2(6.04, 3.61));
	vd.insert(Site_2(-1.03, 5.01));
	vd.insert(Site_2(-6.03, -4.61));
	vd.insert(Site_2(2.09, -1.91));
	assert( vd.is_valid() );

	std::cout << "\n===========\nNumber of Voronoi faces : " << vd.number_of_faces ()  << std::endl;

	VD::Vertex_iterator vit = vd.vertices_begin();

	for(; vit != vd.vertices_end(); vit++)
	{
		std::cout << vit->point()  << std::endl;
	}

	//get infinate edges in VD and clip by the geometry imported from FBX file
	//to calculate the boundary

	//............
	std::cout << "\nCalculate the boundary\n" << std::endl;
	VD::Face_iterator fit = vd.faces_begin ();

	//polygon list
	std::vector<CDPolygon_2> polygonList;

	//output to FBXmesh
	outputMesh = KFbxMesh::Create(SdkManager, "");
#pragma endregion DonnotCare

	int i = 0; //mesh control point index;

	for(; fit != vd.faces_end(); fit++)
	{
		//unbounded case
		if(fit->is_unbounded())
		{
			VD::Face::Ccb_halfedge_circulator c_halfedge = fit->outer_ccb();

			std::cout<<"/////////////////////One Halfedge//////////////////////"<<std::endl;
			std::vector<KFbxVector4> polygonPoints;
			std::vector<KFbxVector4>::iterator it;
			
			bool alreadyGetUnboundEdge = false;
			int firstIntersectEdge = 0;
			int secondIntersectEdge = 0;
			
	
			
			std::vector<CDSegment_2>::iterator firstIntersectSegIt = segmentList.begin();

			shiftToFirstUnboundEdge(c_halfedge);
			VD::Face::Ccb_halfedge_circulator c_halfedge_first = c_halfedge ;

			VD::Face::Ccb_halfedge_circulator temp = c_halfedge;
			//test the result of shiftToFirstUnboundEdge function
			do 
			{
				std::cout<< (temp->is_unbounded()? "unbounded":"bounded") <<std::endl;
				temp++;
			} while (temp != c_halfedge_first);


			do
			{
				//if(!hE.is_valid())
				//	std::cout<<"half edge is not valid!" <<std::endl;

				CDPoint_2 raybase, rayV; //base the origin, point is where the ray point to
				CDDirection_2 rayDirection; // the orientation of the ray 
				CDVector_2 rayVector;


				if(c_halfedge->is_unbounded () )//for the unbounded halfedge in unbounded face
				{
					
					
#pragma region Get Ray
					//std::cout<<"Unbounded ";
					//std::cout<<"Is the halfedge a  ray : "<< ( c_halfedge->is_ray() ? "True": "False") << std::endl;
					if(c_halfedge->has_source())
					{
						CDPoint_2 temp( c_halfedge->source()->point().x(), c_halfedge->source()->point().y()) ;
						raybase = temp;
						std::cout<<"Source : "<< raybase << std::endl;

						CDPoint_2 up(c_halfedge->up()->point().x(),c_halfedge->up()->point().y());
						CDPoint_2 down(c_halfedge->down()->point().x(), c_halfedge->down()->point().y());

						//transform direction clockwise by 90 degree to get the real direction vector of the ray
						//rayDirection = CDDirection_2(CDSegment_2(down, up));
						rayVector = CDSegment_2(down, up).to_vector ().perpendicular (CGAL::CLOCKWISE);
						
						std::cout<<"Up: " << up << "   Down: " << down << std::endl;

					}
					else if(c_halfedge->has_target())
					{
						CDPoint_2 temp( c_halfedge->target()->point().x(), c_halfedge->target()->point().y()) ;
						raybase = temp;
						std::cout<<"Target : "<< raybase << std::endl;
						//calculate the mid point of two nearby site

						//because this halfedge has target so, switch up and down
						CDPoint_2 down(c_halfedge->up()->point().x(),c_halfedge->up()->point().y());
						CDPoint_2 up(c_halfedge->down()->point().x(), c_halfedge->down()->point().y());

						//transform direction clockwise by 90 degree to get the real direction vector of the ray
						//rayDirection = CDDirection_2(CDSegment_2(down, up));
						rayVector = CDSegment_2(down, up).to_vector ().perpendicular (CGAL::CLOCKWISE);

						std::cout<<"Up: " << up << "   Down: " << down << std::endl;

					}
					
					//calculate the intersection
					//const CDSegment_2 *intersectSeg;

					CDRay_2 ray(raybase, rayVector.direction () );
					assert( !ray.is_degenerate () );	
#pragma endregion Get Ray


					//intersection object 
					std::cout << "Calculate Intersection =====================" << std::endl;

					const CDPoint_2 *intersectPoint;

					std::vector<CDSegment_2>::iterator segit = segmentList.begin();
					//std::vector<CDSegment_2>::iterator firstHit;
					CGAL::Object obj;

					if(!alreadyGetUnboundEdge)
					{
						if( !getRaySegmentsIntersect(ray, segmentList, intersectPoint, segit, obj)/*isPointInPolygon(ray.source(), pgn)*/ )
						{
							//find previous intersection
							VD::Face::Ccb_halfedge_circulator temp = c_halfedge;
							CGAL::Object obj;
							const CDPoint_2 *intersectPoint;
							bool intersected = false;
							std::vector<CDSegment_2>::iterator segit;
							do 
							{
								--temp;
								CDPoint_2 pTarget = CDPoint_2(temp->target()->point().x(), temp->target()->point().y());
								CDPoint_2  pSource =  CDPoint_2(temp->source()->point().x(), temp->source()->point().y());
								CDSegment_2 segTemp = CDSegment_2(pSource, pTarget);
								segit = segmentList.begin();
								intersected = getSegmentSegmentsIntersect(segTemp, segmentList, intersectPoint, segit, obj);

							} while (!intersected);

							KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
							polygonPoints.push_back(P);

							//update
							firstIntersectSegIt = segit;
							alreadyGetUnboundEdge = true;

							c_halfedge++;
							continue;
						}

						assert(getRaySegmentsIntersect(ray, segmentList, intersectPoint, segit, obj));
						assert(c_halfedge->source().operator->());
						KFbxVector4 P0(c_halfedge->source()->point().x(), c_halfedge->source()->point().y(), 0,0);
						polygonPoints.push_back(P0);


						firstIntersectSegIt = segit;

						assert(firstIntersectSegIt.operator ->());
						alreadyGetUnboundEdge = true;
						std::cout<<"firstIntersectSeg -> target: "<<CDPoint_2(firstIntersectSegIt->target().x(), firstIntersectSegIt->target().y())<<std::endl;
						std::cout<<"firstIntersectSeg -> source: "<<CDPoint_2(firstIntersectSegIt->source().x(), firstIntersectSegIt->source().y())<<std::endl;

						KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
						polygonPoints.push_back(P);


					}
					else
					{
						if(!getRaySegmentsIntersect(ray, segmentList, intersectPoint, segit, obj)) //when the second unbounded edge is out of the boundary
						{
							//find next intersection
							VD::Face::Ccb_halfedge_circulator temp = c_halfedge;
							CGAL::Object obj;
							//const CDPoint_2 *intersectPoint;
							bool intersected = false;
							//std::vector<CDSegment_2>::iterator segit;
							do 
							{
								++temp;
								CDPoint_2 pTarget = CDPoint_2(temp->target()->point().x(), temp->target()->point().y());
								CDPoint_2  pSource =  CDPoint_2(temp->source()->point().x(), temp->source()->point().y());
								CDSegment_2 segTemp = CDSegment_2(pSource, pTarget);
								segit = segmentList.begin();
								intersected = getSegmentSegmentsIntersect(segTemp, segmentList, intersectPoint, segit, obj);

							} while (!intersected);

							if (segit == firstIntersectSegIt)//if the intersections are on the same edge, this unbounded half edge can be ignored
							{
								///do nothing
							}
							else
							{
								do 
								{
									if(firstIntersectSegIt != segmentList.end() - 1 )
										firstIntersectSegIt++;
									else
										firstIntersectSegIt = segmentList.begin();

									KFbxVector4 P0(firstIntersectSegIt->source().x(), firstIntersectSegIt->source().y(), 0, 0);
									polygonPoints.push_back(P0);

								} while (firstIntersectSegIt != segit);

							}


							c_halfedge++;
							continue;
						}
						
						std::cout<<"segit -> target: "<<CDPoint_2(segit->target().x(), segit->target().y())<<std::endl;
						std::cout<<"segit -> source: "<<CDPoint_2(segit->source().x(), segit->source().y())<<std::endl;

						if(firstIntersectSegIt == segit)
						{
							KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
							polygonPoints.push_back(P);
						}
						else // when unbounded edge intersected two different boundary edge
						{
							do 
							{
								if(firstIntersectSegIt != segmentList.end() - 1 )
									firstIntersectSegIt++;
								else
									firstIntersectSegIt = segmentList.begin();

								KFbxVector4 P0(firstIntersectSegIt->source().x(), firstIntersectSegIt->source().y(), 0, 0);
								polygonPoints.push_back(P0);

							} while (firstIntersectSegIt != segit);

							KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
							polygonPoints.push_back(P);
						}
					}
				
				}
				else//for the bounded halfedge
				{
					
					//case: when the source or target is out of the boundary
					CDPoint_2 pTarget = CDPoint_2(c_halfedge->target()->point().x(), c_halfedge->target()->point().y());
					CDPoint_2  pSource =  CDPoint_2(c_halfedge->source()->point().x(), c_halfedge->source()->point().y());

					CDSegment_2 segTemp = CDSegment_2(pSource, pTarget);
					CGAL::Object obj;
					const CDPoint_2 *intersectPoint;
					std::vector<CDSegment_2>::iterator segit = segmentList.begin();

					if(getSegmentSegmentsIntersect(segTemp, segmentList, intersectPoint, segit, obj))
					{
						if( isPointInPolygon(CDPoint_2(c_halfedge->source()->point().x(), c_halfedge->source()->point().y()), pgn))
						{
							KFbxVector4 P0(c_halfedge->source()->point().x(), c_halfedge->source()->point().y(), 0, 0);
							polygonPoints.push_back(P0);
						}

						KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
						polygonPoints.push_back(P);
			
					}
					else
					{
						if ( isPointInPolygon(CDPoint_2(c_halfedge->source()->point().x(), c_halfedge->source()->point().y()), pgn)
							&& isPointInPolygon(CDPoint_2(c_halfedge->target()->point().x(), c_halfedge->target()->point().y()), pgn))
						{
							KFbxVector4 P(c_halfedge->source()->point().x(), c_halfedge->source()->point().y(), 0, 0);
							polygonPoints.push_back(P);
						}

					}
		
				}

				c_halfedge++;

			}
			while (c_halfedge != c_halfedge_first);
			if(polygonPoints.size()>=3)
				fbx.addPolygonObjectToScene(polygonPoints);				
		}
		else //bounded case
		{
			VD::Face::Ccb_halfedge_circulator c_halfedge = fit->outer_ccb();
			VD::Face::Ccb_halfedge_circulator c_halfedge_first = c_halfedge ;
			//fbx data structure
			std::vector<KFbxVector4> polygonPoints;
			std::vector<KFbxVector4>::iterator it;

			//divided into two case
			std::cout<<"//////////Bounded Face//////////////"<<std::endl;
			do
			{
				//@@@@@@@TODO: more case need to be considered , two adjacent bounded edge intersect two different edge on the boundary
				CDPoint_2 pTarget = CDPoint_2(c_halfedge->target()->point().x(), c_halfedge->target()->point().y());
				CDPoint_2  pSource =  CDPoint_2(c_halfedge->source()->point().x(), c_halfedge->source()->point().y());

				CDSegment_2 segTemp = CDSegment_2(pSource, pTarget);
				CGAL::Object obj;
				const CDPoint_2 *intersectPoint;
				std::vector<CDSegment_2>::iterator segit = segmentList.begin();

				if(getSegmentSegmentsIntersect(segTemp, segmentList, intersectPoint, segit, obj))
				{
					if( isPointInPolygon(CDPoint_2(c_halfedge->source()->point().x(), c_halfedge->source()->point().y()), pgn))
					{
						KFbxVector4 P0(c_halfedge->source()->point().x(), c_halfedge->source()->point().y(), 0, 0);
						polygonPoints.push_back(P0);
					}

					KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
					polygonPoints.push_back(P);

				}
				else
				{
					if ( isPointInPolygon(CDPoint_2(c_halfedge->source()->point().x(), c_halfedge->source()->point().y()), pgn)
						&& isPointInPolygon(CDPoint_2(c_halfedge->target()->point().x(), c_halfedge->target()->point().y()), pgn))
					{
						KFbxVector4 P(c_halfedge->source()->point().x(), c_halfedge->source()->point().y(), 0, 0);
						polygonPoints.push_back(P);
					}
				}

				c_halfedge++;

			}
			while (c_halfedge != c_halfedge_first);
			std::cout<<"//////////Bounded Face END//////////////\n"<<std::endl;


			fbx.addPolygonObjectToScene(polygonPoints);
			
		}
	}

	//std::cout << "OutputMesh Polygon Count: " << outputMesh->GetPolygonCount ()<<std::endl;
	//std::cout << "OutputMesh GetPolygonSize Count: " << outputMesh->GetPolygonSize (0)<<std::endl;
	//KFbxVector4* op_CP = outputMesh->GetControlPoints();

	//std::cout << op_CP[0][0] << " " << op_CP[0][1] << " " << op_CP[0][2] << std::endl;
 //	std::cout << op_CP[1][0] << " " << op_CP[1][1] << " " << op_CP[1][2] << std::endl;
	//std::cout << op_CP[2][0] << " " << op_CP[2][1] << " " << op_CP[2][2] << std::endl;
	//std::cout << op_CP[3][0] << " " << op_CP[3][1] << " " << op_CP[3][2] << std::endl;

	//outPutNode = KFbxNode::Create(SdkManager, "");
	//assert(outPutNode->AddNodeAttribute(outputMesh));
	//std::cout << "outPutNode Attribute Count: " << outPutNode->GetNodeAttributeCount ()<<std::endl;
	//KFbxNode *temproot = outPutScene->GetRootNode();
	//assert(temproot->AddChild(outPutNode));
	   
	//Result = SaveScene(SdkManager,outPutScene, "Shatter.fbx", -1, false);
	fbx.SaveScene("shatter_fbx.fbx");


	return 0;
}

