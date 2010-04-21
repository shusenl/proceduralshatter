// fbxShatterApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" bool SaveScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat, bool pEmbedMedia);
extern "C" bool LoadScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename);


struct Voronoi_recup 
{ //class using stream to get the voronoi diagram
	RayList quR;
	LineList quL;
	SegmentList quS;

	Voronoi_recup() {}

	void operator<<(const Ray p){quR.insert(quR.begin(),p);}
	void operator<<(const Line p){quL.insert(quL.begin(),p);}
	void operator<<(const Segment p){quS.insert(quS.begin(),p);}

};

//Convert objects into drawable segments
template<class iterator,class lst>
void cast_into_seg(const iterator first,const iterator end,const Rectangle& bbox,lst& Seglist)
{
	for (iterator it=first;it!=end;++it)
	{
		CGAL::Object obj_cgal = CGAL::intersection(*it,bbox);
		Segment s;
		if (CGAL::assign(s, obj_cgal))
			Seglist.push_back(s);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	
    KFbxSdkManager* SdkManager = NULL;
    KFbxScene* Scene = NULL, *outPutScene = NULL;
	KFbxNode* Node = NULL, *outPutNode = NULL;
	KFbxImporter* IImporter = NULL;
	KFbxExporter* IExporter = NULL;
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
	
	KFbxGeometry *plane;// = KFbxGeometry::Create(SdkManager, "plane");
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


	/////////////CGAL/////////////////

	VD vd;
	Site_2 t1(2, 2);
	Site_2 t2(-2, -2);
	Site_2 t3(2, -2);
	Site_2 t4(-2,2);
	Site_2 t5(0,0);
	vd.insert(t1);
	vd.insert(t2);
	vd.insert(t3);
	vd.insert(t4);
	vd.insert(t5);

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

	int i = 0; //mesh control point index;
	for(; fit != vd.faces_end(); fit++)
	{
		//unbounded case!!!!!!!!!!!!!!! What the FUCK !!!!!!!!!!!!!!!
		if(fit->is_unbounded())
		{
			VD::Face::Ccb_halfedge_circulator c_halfedge = fit->outer_ccb();
			VD::Face::Ccb_halfedge_circulator c_halfedge_first = c_halfedge ;

			do
			{
				//VD::Face::Halfedge hE = *c_halfedge ;//= boost::get<Halfedge_handle>(&c_halfedge) ;
				//if(!hE.is_valid())
				//	std::cout<<"half edge is not valid!" <<std::endl;

				
				
				CDPoint_2 raybase, raypoint; //base the origin, point is where the ray point to
				if(c_halfedge->is_unbounded () )
				{
					
					
					//std::cout<<"Unbounded ";
					//std::cout<<"Is the halfedge a  ray : "<< ( c_halfedge->is_ray() ? "True": "False") << std::endl;
					if(c_halfedge->has_source())
					{
						CDPoint_2 temp( c_halfedge->source()->point().x(), c_halfedge->source()->point().y()) ;
						raybase = temp;
						std::cout<<"Source : "<< raybase << std::endl;
						//calculate the mid point of two nearby site
						//c_halfedge->dual () 

						//std::cout<<"duel : " <<(*(c_halfedge->dual().first)).point() <<std::endl;//.first->point() << c_halfedge->dual().second->point() <<std::endl;

					}
					if(c_halfedge->has_target())
					{
						CDPoint_2 temp( c_halfedge->target()->point().x(), c_halfedge->target()->point().y()) ;
						raybase = temp;
						std::cout<<"Target : "<< raybase << std::endl;
						//calculate the mid point of two nearby site
						CDPoint_2 
							midpoint( 
							(c_halfedge->right()->point().x() + c_halfedge->opposite()->left()->point().x())/2.0,
							(c_halfedge->right()->point().y() + c_halfedge->opposite()->left()->point().y())/2.0
							);
						std::cout<<"Midpoint: " <<c_halfedge->up()->point()<<std::endl;

					}
					
					CDRay_2 ray;
				}

				c_halfedge++;

			}
			while (c_halfedge != c_halfedge_first);
				
		}
		else //bounded case
		{
			VD::Face::Ccb_halfedge_circulator c_halfedge = fit->outer_ccb();
			VD::Face::Ccb_halfedge_circulator c_halfedge_first = c_halfedge ;
			//fbx datastructure
			std::vector<KFbxVector4> polygonPoints;
			std::vector<KFbxVector4>::iterator it;
			do
			{
				//c_halfedge->source()->point();

				KFbxVector4 P(c_halfedge->source()->point().x(), c_halfedge->source()->point().y(), 0, 0);
				polygonPoints.push_back(P);


				c_halfedge++;

			}
			while (c_halfedge != c_halfedge_first);
			
			outputMesh->BeginPolygon();
			for (it = polygonPoints.begin(); it != polygonPoints.end(); it++, i++)
			{
				outputMesh->SetControlPointAt((*it), i);
				outputMesh->AddPolygon(i);
			}
			outputMesh->EndPolygon();
			outputMesh->BuildMeshEdgeArray();
			//CDPolygon_2 pgn;
			//pgn.insert()
			
		}
	}

	std::cout << "OutputMesh Polygon Count: " << outputMesh->GetPolygonCount ()<<std::endl;
	std::cout << "OutputMesh GetPolygonSize Count: " << outputMesh->GetPolygonSize (0)<<std::endl;
	KFbxVector4* op_CP = outputMesh->GetControlPoints();

	std::cout << op_CP[0][0] << " " << op_CP[0][1] << " " << op_CP[0][2] << std::endl;
 	std::cout << op_CP[1][0] << " " << op_CP[1][1] << " " << op_CP[1][2] << std::endl;
	std::cout << op_CP[2][0] << " " << op_CP[2][1] << " " << op_CP[2][2] << std::endl;
	std::cout << op_CP[3][0] << " " << op_CP[3][1] << " " << op_CP[3][2] << std::endl;

	/*
	//=============================================================
	//another approach by using dual of the DT
	CGAL::Delaunay_triangulation_2<KernelCD> dt;
	Voronoi_recup v_recup = Voronoi_recup();

	DTPoint_2 dts1(1.0, 2.3);
	DTPoint_2 dts2(-1.0, -1.0);
	DTPoint_2 dts3(1.0, -2.3);
	DTPoint_2 dts4(0,0);

	dt.insert(dts1);
	dt.insert(dts2);
	dt.insert(dts3);
	dt.insert(dts4);
	//add your points here

	assert(  dt.is_valid() );

	//CGAL::Delaunay_triangulation_2<KernelCD>::Finite_faces_iterator

	dt.draw_dual(v_recup);

	CDPoint_2 bb1( p_CP[0][0],  p_CP[0][2]);
	CDPoint_2 bb2( p_CP[1][0],  p_CP[1][2]);
	CDPoint_2 bb3( p_CP[2][0],  p_CP[2][2]);
	CDPoint_2 bb4( p_CP[3][0],  p_CP[3][2]);

	Rectangle bbox_cgal(bb2, bb3);

	//SL is segment list
	SegmentList SL;

	cast_into_seg(v_recup.quR.begin(),v_recup.quR.end(),bbox_cgal,SL);//cast rays into segments in bbox
	cast_into_seg(v_recup.quL.begin(),v_recup.quL.end(),bbox_cgal,SL);//cast lines into segments in bbox
	cast_into_seg(v_recup.quS.begin(),v_recup.quS.end(),bbox_cgal,SL);//cast lines into segments in bbox

	int segmentSize = SL.size();
	std::cout << "Size of SegmentList: " << segmentSize << std::endl;
	SegmentList::iterator sg_it = SL.begin();
	for(; sg_it != SL.end(); sg_it++)
	{
		std::cout << "Point 1 :" <<sg_it->point(0)[0] <<" "<< sg_it->point(0)[1] << "    Point 2:"<<sg_it->point(1)[0] <<" "<< sg_it->point(1)[1] <<std::endl;
	}
	*/

	//====================================================
	//save to scene

	outPutNode = KFbxNode::Create(SdkManager, "");
	assert(outPutNode->AddNodeAttribute(outputMesh));
	std::cout << "outPutNode Attribute Count: " << outPutNode->GetNodeAttributeCount ()<<std::endl;
	KFbxNode *temproot = outPutScene->GetRootNode();
	assert(temproot->AddChild(outPutNode));
	   
	Result = SaveScene(SdkManager,outPutScene, "Shatter.fbx", -1, false);



	return 0;
}

