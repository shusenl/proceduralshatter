// fbxShatterApp.cpp : Defines the entry point for the console application.
// Prototype 4-21-2010 by Shusen Liu, University of Utah

#include "stdafx.h"

extern "C" bool SaveScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat, bool pEmbedMedia);
extern "C" bool LoadScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename);

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

void shiftToFirstUnboundEdge(VD::Face::Ccb_halfedge_circulator &c_halfedge)
{
	bool founded = false;
	VD::Face::Ccb_halfedge_circulator c_temp;
	while( !founded )
	{ 
		c_temp = c_halfedge++;
		if(c_temp->is_unbounded () && (++c_temp)->is_unbounded())
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
	//vd.insert(Site_2(3.66, 2.1));
	vd.insert(Site_2(4.2, -3.1));
	vd.insert(Site_2(1.4, 1.1));
	vd.insert(Site_2(-1.3, 2.1));
	//vd.insert(Site_2(-5.3, -4.1));
	//vd.insert(Site_2(-3.9, -0.1));
	//for(int i=-3; i<4; i++)
	//{
	//	for(int j=-3; j<4; j++)
	//	{
	//		srand((unsigned)i); 
	//		double random_x = double(rand())/RAND_MAX *  1000.0;
	//		srand((unsigned)j + 1); 
	//		double random_y = double(rand())/RAND_MAX *  1000.0;
	//		std::cout<< i + random_x << " " <<  j + random_y << std::endl;
	//		vd.insert(Site_2(i + random_x, j + random_y));
	//	}

	//}

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


				if(c_halfedge->is_unbounded () )
				{
					
					
#pragma region Get Ray
					//std::cout<<"Unbounded ";
					//std::cout<<"Is the halfedge a  ray : "<< ( c_halfedge->is_ray() ? "True": "False") << std::endl;
					if(c_halfedge->has_source())
					{
						CDPoint_2 temp( c_halfedge->source()->point().x(), c_halfedge->source()->point().y()) ;
						raybase = temp;
						std::cout<<"Source : "<< raybase << std::endl;
						//add the first vertex in the polygon
						KFbxVector4 P(temp.x(), temp.y(), 0, 0);
						polygonPoints.push_back(P);

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
						assert(getRaySegmentsIntersect(ray, segmentList, intersectPoint, segit, obj));

						firstIntersectSegIt = segit;

						assert(firstIntersectSegIt.operator ->());
						alreadyGetUnboundEdge = true;
						std::cout<<"firstIntersectSeg -> target: "<<CDPoint_2(firstIntersectSegIt->target().x(), firstIntersectSegIt->target().y())<<std::endl;
						std::cout<<"firstIntersectSeg -> source: "<<CDPoint_2(firstIntersectSegIt->source().x(), firstIntersectSegIt->source().y())<<std::endl;
						//if (firstIntersectSegIt != segmentList.end()-1)
						//{
						//	++firstIntersectSegIt;
						//	std::cout<<*firstIntersectSegIt<<std::endl;
						//	//std::cout<<"firstIntersectSeg -> target: "<<CDPoint_2(firstIntersectSegIt->target().x(), firstIntersectSegIt->target().y())<<std::endl;
						//	//std::cout<<"firstIntersectSeg -> source: "<<CDPoint_2(firstIntersectSegIt->source().x(), firstIntersectSegIt->source().y())<<std::endl;
						//}


						KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
						polygonPoints.push_back(P);


					}
					else
					{
						assert(getRaySegmentsIntersect(ray, segmentList, intersectPoint, segit, obj));
						std::cout<<"segit -> target: "<<CDPoint_2(segit->target().x(), segit->target().y())<<std::endl;
						std::cout<<"segit -> source: "<<CDPoint_2(segit->source().x(), segit->source().y())<<std::endl;

						if(firstIntersectSegIt == segit)
						{
							KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
							polygonPoints.push_back(P);
						}
						else
						{
							//TODO: make more general case
							if(firstIntersectSegIt != segmentList.end() - 1 )
								firstIntersectSegIt++;
							else
								firstIntersectSegIt = segmentList.begin();
							assert(firstIntersectSegIt.operator ->());
							std::cout<<"firstIntersectSegIt++ -> target: "<<CDPoint_2(firstIntersectSegIt->target().x(), firstIntersectSegIt->target().y())<<std::endl;
							std::cout<<"firstIntersectSegIt++ -> source: "<<CDPoint_2(firstIntersectSegIt->source().x(), firstIntersectSegIt->source().y())<<std::endl;

							KFbxVector4 P0(firstIntersectSegIt->source().x(), firstIntersectSegIt->source().y(), 0, 0);
							polygonPoints.push_back(P0);

							if(firstIntersectSegIt != segmentList.end()- 1 )
								firstIntersectSegIt++;
							else
								firstIntersectSegIt = segmentList.begin();

							if (firstIntersectSegIt == segit)
							{
								KFbxVector4 P(firstIntersectSegIt->source().x(), firstIntersectSegIt->source().y(), 0, 0);
								polygonPoints.push_back(P);

							}

							KFbxVector4 P(intersectPoint->x(), intersectPoint->y(), 0, 0);
							polygonPoints.push_back(P);

						}
						


					}
					
		


				
				}
				else
				{
					//for the bounded halfedge
					KFbxVector4 P1(c_halfedge->source()->point().x(), c_halfedge->source()->point().y(), 0, 0);
					polygonPoints.push_back(P1);

					KFbxVector4 P2(c_halfedge->target()->point().x(), c_halfedge->target()->point().y(), 0, 0);
					//polygonPoints.push_back(P2);
				
				}
				c_halfedge++;

			}
			while (c_halfedge != c_halfedge_first);
			
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
			do
			{
				//c_halfedge->source()->point();

				//point is outside the boundary
				if(c_halfedge->source()->point().x() > isoRect.xmax() || c_halfedge->source()->point().x() < isoRect.xmin() ||
				   c_halfedge->source()->point().y() > isoRect.ymax() || c_halfedge->source()->point().y() < isoRect.ymin())
				{
					CDPoint_2 pTarget = CDPoint_2(c_halfedge->target()->point().x(), c_halfedge->target()->point().y());
					CDPoint_2  pSource =  CDPoint_2(c_halfedge->source()->point().x(), c_halfedge->source()->point().y());

					CDSegment_2 segTemp = CDSegment_2(pSource, pTarget);
					
					CGAL::Object obj;
					const CDPoint_2 *intersectPoint;
					KFbxVector4 P;
					std::vector<CDSegment_2>::iterator segit = segmentList.begin();
					for(; segit != segmentList.end(); segit++)
					{
						obj = intersection(segTemp, *segit);
						if ( intersectPoint = CGAL::object_cast<CDPoint_2>(&obj)) 
						{
							std::cout<< "@@@Intersect At: "<< *intersectPoint <<"\n\n"<< std::endl;
							P.Set(intersectPoint->x(), intersectPoint->y(), 0, 0);
							polygonPoints.push_back(P);
							break;
						}
					}
				}


				//point is outside the boundary
				if(c_halfedge->target()->point().x() > isoRect.xmax() || c_halfedge->target()->point().x() < isoRect.xmin() ||
					c_halfedge->target()->point().y() > isoRect.ymax() || c_halfedge->target()->point().y() < isoRect.ymin())
				{
					CDPoint_2 pTarget = CDPoint_2(c_halfedge->target()->point().x(), c_halfedge->target()->point().y());
					CDPoint_2  pSource =  CDPoint_2(c_halfedge->source()->point().x(), c_halfedge->source()->point().y());

					CDSegment_2 segTemp = CDSegment_2(pSource, pTarget);

					CGAL::Object obj;
					const CDPoint_2 *intersectPoint;
					KFbxVector4 P;
					std::vector<CDSegment_2>::iterator segit = segmentList.begin();
					for(; segit != segmentList.end(); segit++)
					{
						obj = intersection(segTemp, *segit);
						if ( intersectPoint = CGAL::object_cast<CDPoint_2>(&obj)) 
						{
							std::cout<< "@@@Intersect At: "<< *intersectPoint <<"\n\n"<< std::endl;
							P.Set(intersectPoint->x(), intersectPoint->y(), 0, 0);
							polygonPoints.push_back(P);
							break;
						}
					}
				}

				KFbxVector4 P(c_halfedge->source()->point().x(), c_halfedge->source()->point().y(), 0, 0);
				polygonPoints.push_back(P);

				c_halfedge++;

			}
			while (c_halfedge != c_halfedge_first);
			

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

